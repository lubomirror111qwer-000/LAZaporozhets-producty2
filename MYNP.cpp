/* Облік продуктів Додавання продукту (назва, кількість, термін придатності);
перегляд; попередження про закінчення терміну */
#include<iostream>
#include<iomanip>// для таблиці
#include<cstring>
#include<vector>
#include "MYNP.h"
#include <fstream>//для файлу
#include <algorithm>

using namespace std;


 
/**Функція відкриває текстовий файл для запису та зберігає в нього всі
продукти з вектора. Кожен продукт записується в окремий рядок,
а його властивості розділяються пробілами. Якщо файл уже існував,
його вміст повністю перезаписується актуальними даними.
Вхідні дані (параметри)
const string& filename — ім'я або шлях до файлу куди будуть збережені дані.
const vector<Row>& product — посилання на вектор масив структур продуктів,
які потрібно записати. const гарантує, що функція випадково
не змінить дані в пам'яті під час запису.***/
void save_data(const string& filename, const vector<Row>& product) 
{
    // Відкриваємо файл для запису 
    ofstream f(filename); 
    
    if (!f.is_open())
    {
        cout << " Не вдалося відкрити файл для запису!" << endl;
        return;
    }

    // Записуємо кожен продукт окремим рядком
    for (const auto& item : product) 
    {
        f << item.id << " " 
          << item.Name << " " 
          << item.k << " " 
          << item.data.day << " " 
          << item.data.mon << " " 
          << item.data.year << "\n";
    }

    f.close(); // Обов'язково закриваємо файл
    cout << "Дані успішно збережено у файл " << filename << "!" << endl;
}

// завантаження даних з файлу
/****опис :Функція відкриває текстовий файл для читання та автоматично завантажує 
з нього дані про продукти назад у програму.  Зчитує файл рядок за рядком,
вхідні дані
const string& filename — ім'я файлу "file.txt" за замовчуванням
*/
vector<Row> load_data(const string& filename) 
{
    vector<Row> product;
    // Відкриваємо файл для зчитування
    ifstream f(filename); 

    // Якщо файл не існує — повертаємо порожній продукт
    if (!f.is_open()) 
    {
        return product; 
    }

    Row item;
    // Зчитуємо дані покроково, поки не дійдемо до кінця файлу
    while (f >> item.id >> item.Name >> item.k >> item.data.day >> item.data.mon >> item.data.year)
    {
        product.push_back(item);
    }

    f.close();
    return product;
}
/*Функція відповідає за додавання (введення з клавіатури) одного нового продукту. 
послідовно запитує у користувача ID, назву,
кількість та дату терміну придатності. Також функція містить захист від некоректного вводу

Вхідні дані
 поступове зчитування через cin*/
Row newproduct()
{
    Row product;
    setlocale(LC_ALL, "Ucraine");
    
    // Початково маркуємо ID як помилковий
    product.id = -1; 

    cout << "Заповнення рядка         " << endl;
    cout << "Введіть код              ";  cin >> product.id;
             if (cin.fail()) goto input_error;
    cout << "Введіть назву продукту   "; cin >> product.Name;
    
    //  захист назви від спеціальнихсимволів (*, /, , і т.д.) та цифр
    for (char c : product.Name)
    {
        // перевіряємо, чи є символ цифрою, знаком пунктуації або спецсимволом
        if (isdigit(c) || ispunct(c) || c == '+' || c == '-' || c == '*' || c == '/')
        {
            cout << "\" Назва продукту не може містити цифри, розділювачі або спецсимволи" << endl;
            goto input_error;
        }
    }
    cout << "Введіть  кількість    :      ";  cin >> product.k;
             if (cin.fail()) goto input_error;
             if (cin.fail() || product.k < 0) goto input_error;
    cout << "Дата терміну придатності:\n";
    cout << "Введіть  число         : ";  cin >> product.data.day;
             if (cin.fail()) goto input_error; // Якщо ввести 'а' тут - програма  зупиниться виддасть помилку перейде до меню
             if (cin.fail() || product.data.day < 1 || product.data.day > 31) goto input_error;
    cout << "Введіть  місяць        : ";   cin >> product.data.mon;
             if (cin.fail()) goto input_error;
             if (cin.fail() || product.data.mon < 1 || product.data.mon > 12) goto input_error;
    cout << "Введіть  рік           : ";  cin >> product.data.year;
             if (cin.fail()) goto input_error;

    // Якщо все пройшло успішно, повертаємо заповнений продукт
    return product;

input_error: 
    cout << "\n Ви ввели некоректні дані літеру замість числа або крапку" << endl;
    cout << "в терміні придатності вводьте  цілі числа. через enter" << endl;
    
    cin.clear();            // Скидаємо прапорець помилки cin
    cin.ignore(1000, '\n'); // Повністю очищаємо буфер від сміття
    
    product.id = -1; //  сталася помилка, дані введено некоректно
    return product;
}
/*Функція  перегляду 
опис: структуроване виведення всього списку продуктів на екран у вигляді таблиці.
Вона використовує маніпулятори форматування (setw, left, right), 
щоб вирівняти колонки коду, назви й кількості. Також вона автоматично
підставляє нулі попереду для одноцифрових днів та місяців
вхідні дані 
const vector<Row>& product — посилання на вектор масив структур продуктів,
*/
void showall(const vector<Row>& product)
{
    // Якщо продукт порожній, попереджаємо користувача
    if (product.empty()) 
    {
        cout << "\nрядок продуктів порожній!" << endl;
        return;
    }

    cout << "\n======перегляд продуктів =======\n";
    
    // Заголовок таблиці (ширина колонок: id=8, Name=25, k=12)
    cout << left << setw(8) << "Код" 
         << setw(30) << "Назва продукту" 
         << setw(17) << "Кількість" 
         <<setw(10) <<"     Термін придатності" << endl;
    cout << string(60, '-') << endl; // Малює лінію з 60 дефісів

    // Циклом перебираємо кожен продукт у векторі
    for (const auto& item : product)
    {
        cout << left << setw(8) << item.id 
             << setw(25) << item.Name 
             << setw(12) << item.k;
        cout << right;
        // Виведення дати у красивому форматі дд.мм.рррр із нулями попереду
    cout << setfill('0') << setw(2) << item.data.day << setfill(' ') << ".";
    cout << setfill('0') << setw(2) << item.data.mon << setfill(' ') << ".";
    cout << item.data.year << endl;
    }
    cout << string(60, '-') << endl;
}

/*Функція аналізує всю базу продуктів і виявляє продукти,
порівнюючи їхній термін придатності з поточною датою,
яку користувач вводить вручну. Функція працює за двома
критеріями: вона знаходить повністю прострочені товари
та критичні товари у яких термін закінчується протягом
найближчих 3 днів або на самому початку наступного місяця.
вхідні дані 
const vector<Row>& product — посилання на вектор масив структур продуктів
*/
void checkexpiration(const vector<Row>& product) 
{
    if (product.empty())
    {
        cout << "\nядок продуктів порожній. Нічого перевіряти." << endl;
        return;
    }

    int cur_day, cur_mon, cur_year;
    cout << "\n= перевірка термінів придатності =" << endl;
    cout << "Введіть поточний день (1-31): "; cin >> cur_day;
    cout << "Введіть поточний місяць (1-12): "; cin >> cur_mon;
    cout << "Введіть поточний рік (напр. 2026): "; cin >> cur_year;

    if (cin.fail() || cur_mon < 1 || cur_mon > 12 || cur_day < 1 || cur_day > 31)
    {
        cout << "\n Помилка некоректна дата " << endl;
        cin.clear();
        cin.ignore(1000, '\n');
        return;
    }

    cout << "\n==  попередження про закінченя терміну або прострочений  ==\n";
    cout << left << setw(8) << "Код" 
         << setw(25) << "  Назва продукту" 
         << setw(15) << "      Кількість" 
         << "     Термін придатності" << endl;
    cout << string(70, '-') << endl;

    int expired_count = 0;

    for (const auto& item : product)
    {
        bool is_expired = false;
        bool is_critical = false;

        // Перевірка на  прострочення 
        if (item.data.year < cur_year)
        {
            is_expired = true;
        } 
        else if (item.data.year == cur_year)
        {
            if (item.data.mon < cur_mon)
            {
                is_expired = true;
            } 
            else if (item.data.mon == cur_mon && item.data.day <= cur_day)
            {
                is_expired = true;
            }
        }

        //  Перевірка на критичний термін якщо продукт придатний, але лишилося мало часу
        //   день ще не настав 
        //  товар дійсний у наступному місяці, але сьогодні вже кінець поточного 
        if (!is_expired) 
        {
            // Термін цього ж місяця, але за кілька днів (наприклад, сьогодні 25.06, термін 28.06)
            if (item.data.year == cur_year && item.data.mon == cur_mon && (item.data.day - cur_day <= 3))
            {
                is_critical = true;
            }
            //  Сьогодні кінець місяця 28-31 число, а термін — перші числа наступного місяця
            else if (item.data.year == cur_year && (item.data.mon - cur_mon == 1) && cur_day >= 28 && item.data.day <= 3)
            {
                is_critical = true;
            }
        }

        // Виводення товару, якщо він або прострочений, або критичний
        if (is_expired || is_critical) 
        {
            cout << left << setw(8) << item.id 
                 << setw(25) << item.Name 
                 << setw(15) << item.k; 
            
            cout << "    " << right; 
            cout << setfill('0') << setw(2) << item.data.day << setfill(' ') << ".";
            cout << setfill('0') << setw(2) << item.data.mon << setfill(' ') << ".";
            cout << item.data.year;
            
            // Вивід тексту
            if (is_expired)
            {
                cout << "  прострочено";
            } else if (is_critical)
            {
                cout << "   Закінчення терміну придатності";
            }
            
            cout << left << endl;
            expired_count++;
        }
    }

    cout << string(70, '-') << endl;
    if (expired_count == 0)
    {
        cout << " продуктів з простроченим чи закінчення терміну не знайдено." << endl;
    } else 
    {
        cout << " Знайдено закінчення придатності  продуктів: " << expired_count << endl;
    }
}
/*доступ до всіх функцій за вибором користувача від 1 до 0**/
void menu()
{
    // Встановлюємо мову для кирилиці
    setlocale(LC_ALL, "Ucraine");
const string filename = "file.txt";
    //  Створити базу даних (вектор продуктів) до початку меню 
    vector<Row> database = load_data(filename);
    int choice;

    do 
    {
        cout << "\n===  МЕНЮ ===" << endl;
        cout << "1. Додати новий продукт" << endl;
        cout << "2. Переглянути всі продукти" << endl;
        cout << "3. перевірка на термін придатності " << endl;
        cout << "4. збереження у файл txt " << endl;
        cout << "5. зчитування з файлу txt " << endl;
        cout << "0. Вихід з програми" << endl;
        cout << "Ваш вибір: ";
        cin >> choice;
 //захист меню 
    if (cin.fail())
    {
        cout << "\n Некоректний вибір Вводьте  цифри" << endl;
        cin.clear();            // Скидання помилки
        cin.ignore(1000, '\n'); // Очищення буфера від літери 
        choice = -1;            // Ставимо значення, яке просто запустить цикл знову
        continue;               // Повертаємось на початок циклу меню
    }

        switch (choice)
        {
            case 1: 
            {
            Row product = newproduct();
    
             // Перевірка, чи успішно пройшло введення
             if (product.id != -1)
             {
             database.push_back(product); // додоаємо продукт
             cout << "\nПродукт успішно додано!" << endl;
             } else
                {
                     cout << "\n Продукт не було додано через помилку введення." << endl;
                }
            break;
        }
            case 2:
                //  виклик функції перегляду
                showall(database); 
             break;
             case 3:
            checkexpiration(database);
             break;
            
             case 4:
            save_data(filename, database);
             break;
             case 5:
             database = load_data(filename);
             if (database.empty()) 
             {
               cout << "\n Файл не знайдено або він порожній." << endl;
              }
                 else 
                  {
                     cout << "\nДані успішно завантажено з файлу!" << endl;
                  }
            break;
            case 0:
                cout << "\nвихід з програми..." << endl;
                database = load_data(filename);
            break;

            default:
                cout << "\nНеправильний вибір! Спробуйте ще раз." << endl;
                break;
        }

    } while (choice != 0); // Цикл крутиться, поки користувач не натисне 0

     return ;
}
