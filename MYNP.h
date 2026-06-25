#ifndef MYNP_H
#define MYNP_H

#include<string>
#include<vector>
using namespace std;
struct Data
{   
    int  day;
    int  mon;
    int  year;
};

struct Row 
{
    int id;
    string Name;
    int k;
    Data data;// термін придатності
};


struct product
{
    int id;// у код
    string Name;//назва продукту
    int k;// кількість
    Data data;// термін придатності
};
/**************всі функції***********************/
Row newproduct();
void showall(const vector<Row>& product);
void menu();
void checkexpiration(const vector<Row>& product);
void save_data(const string& filename, const vector<Row>& product);
vector<Row> load_data(const string& filename);

#endif
