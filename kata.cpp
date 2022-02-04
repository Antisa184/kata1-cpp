#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <ctime>

#include <Windows.h>
#include <cstdio>

namespace fs = std::filesystem;

using namespace std;

string authorsFirstLine = "email;firstname;lastname";
string booksFirstLine = "title;isbn;authors;description";
string magazinesFirstLine = "title;isbn;authors;publishedAt";

string vectorToString(vector<string> vec){
    string vecString;
    for(string str: vec){
        vecString+=str;
    }
    return vecString;
}

vector<string> split(string phrase, string delimiter){      //split string by delimiter
    vector<string> list;
    string s = phrase;
    size_t pos = 0;
    string token;
    int br=0;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
        br++;
    }
    if (br!=0){
        list.push_back(s);
        return list;
    }
    else{
        list.push_back(phrase);
        return list;
    }
}

struct author{
    string email;
    string firstName;
    string lastName;
    string toString()
  {
    return "Email:\n"+email+"\nFirst name:\n"+firstName+"\nLast name:\n"+lastName+"\n\n";
  }
};

struct readingMaterial{
    virtual ~readingMaterial() {}
    string title;
    string isbn;
    vector<string> authors;
};

struct book: readingMaterial{
    string description;
    string toString()
  {
    return "Title:\n"+title+"\nISBN:\n"+isbn+"\nAuthors:\n"+vectorToString(authors)+"\nDescription:\n"+description+"\n\n";
  }
};

struct magazine: readingMaterial{
    string publishedAt;
    string toString()
  {
    return "Title:\n"+title+"\nISBN:\n"+isbn+"\nAuthors:\n"+vectorToString(authors)+"\nPusblished at:\n"+publishedAt+"\n\n";
  }
};

vector<book*> books;
vector<author*> authors;
vector<magazine*> magazines;
vector<readingMaterial*> readables;

void readAuthors(fstream& file){
    //cout<<"AUTHORS";
    string tp;
    vector<string> tpVector;
    while(getline(file, tp)){
        author* newAuthor= new author();
        tpVector = split(tp, ";");
        newAuthor->email=tpVector[0];
        newAuthor->firstName=tpVector[1];
        newAuthor->lastName=tpVector[2];
        authors.push_back(newAuthor);
        //cout<<newAuthor->toString();
    }
}
void readBooks(fstream& file){
    string tp;
    vector<string> tpVector;
    while(getline(file, tp)){
        book* newBook = new book();
        tpVector = split(tp, ";");
        newBook->title=tpVector[0];
        newBook->isbn=tpVector[1];
        newBook->authors=split(tpVector[2],",");
        newBook->description=tpVector[3];
        books.push_back(newBook);
        readables.push_back(newBook);
        //cout<<newBook->toString();
    }
}
void readMagazines(fstream& file){
    string tp;
    vector<string> tpVector;
    while(getline(file, tp)){
        magazine* newMagazine = new magazine();
        tpVector = split(tp, ";");
        newMagazine->title=tpVector[0];
        newMagazine->isbn=tpVector[1];
        newMagazine->authors=split(tpVector[2],",");
        newMagazine->publishedAt=tpVector[3];
        magazines.push_back(newMagazine);
        readables.push_back(newMagazine);
        //cout<<newMagazine->toString();
    }
}
bool compareReadingMaterialByTitle(const readingMaterial* a, const readingMaterial* b)
    {
    struct readingMaterial *ia = (struct readingMaterial *)a;
    struct readingMaterial *ib = (struct readingMaterial *)b;
    return ia->title < ib->title;
    }

void printReadingMaterial(vector<readingMaterial*> readables, int mode=1, string param=""){
    if(mode==1) sort(readables.begin(),readables.end(),compareReadingMaterialByTitle);
    for (auto r: readables) {
        if( (mode==1 || mode==4) || (mode==2 && r->isbn==param) || (mode==3 && find(r->authors.begin(), r->authors.end(), param)!=r->authors.end())){
            if (book *d = dynamic_cast<book*>(r)) {
                //cout<<"knjiga\n";
                cout<<d->toString();
                cout<<"\n\n";
            }
            if (magazine *d = dynamic_cast<magazine*>(r)) {
                //cout<<"magazin\n";
                cout<<d->toString();
                cout<<"\n\n";
            }
        }
    }
}


int main(){

    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);

    fstream file;
    string path = "C:/Users/antis/Desktop/intervju/data/";
    for(const auto & entry : fs::directory_iterator(path)) //iterate all files in folder
    {
        file.open(entry.path(),ios::in);
        string tp;
        getline(file,tp); //read first line to determine type of data

        //cout<<split(tp,";")[0].size()<<"\n";
        if(split(tp,";")[2]==split(authorsFirstLine,";")[2]) readAuthors(file);
        if(split(tp,";")[3]==split(booksFirstLine,";")[3]) readBooks(file);
        if(split(tp,";")[3]==split(magazinesFirstLine,";")[3]) readMagazines(file);
        file.close(); //close the file object.
    }
    while (true){
        cout<<"Hello!\nTo choose an option write the number associated with the option and press Enter. (eg. 1)\n"
            "1. Print out all books and magazines with all their details.\n"
            "2. Find a book or magazine by its isbn.\n"
            "3. Find all books and magazines by their authors email.\n"
            "4. Print out all books and magazines with all their details sorted by title.\n"
            "Type 'exit' to stop the program.\n";
        string input;
        cin>>input;
        if (input=="exit"){cout<<"Goodbye!\n"; break;}
        if (input=="1"){
            printReadingMaterial(readables);
        }
        if (input=="2"){
            cout<<"Input the isbn: ";
            cin>>input;
            printReadingMaterial(readables, 2, input);
        }
        if (input=="3"){
            cout<<"Input author's email: ";
            cin>>input;
            printReadingMaterial(readables, 3, input);
        }
        if (input=="4"){
            printReadingMaterial(readables,1);
        }
    }
}