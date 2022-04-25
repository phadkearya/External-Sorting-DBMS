#include <bits/stdc++.h>

using namespace std;


class page{
    public:
        page()=delete;
        page(int);
        bool isFull();
        bool insertRecordIntoPage(int);
        void printPage();
        void sortPage();
        void copyContents(vector<int>&);
        virtual ~page();

    protected:

    private:
        int pSize;
        vector <int> records;

};

page::page(int p){
    pSize=p;
}

bool page::isFull(){
    if (records.size()==pSize){
        return true;
    }
    return false;
}

bool page::insertRecordIntoPage(int n){
    records.push_back(n);
    return true;
}

void page::sortPage(){
    sort(records.begin(),records.end());
}

void page::copyContents(vector<int>& v){
    int i;
    for(i=0;i<records.size();i++){
        v.push_back(records[i]);
    }
}

void page::printPage(){
    int i;
    for(i=0;i<records.size();i++){
        cout<<records[i]<<" ";
    }
}

page::~page(){
    //dtor
}

/////////////////////////////////////////////////////////////////////////////


class hFile{
    public:
        hFile()=delete;
        hFile(int,int);
        bool insertRecord(int);
        void printHFile();
        void externalSort();
        void printSortedFile();
        virtual ~hFile();

    protected:

    private:
        int bSize;
        int pageSize;
        vector <page*> buffer;
        vector <page*> pages;

};

hFile::hFile(int b, int p){
    bSize=b;
    pageSize=p;
}

bool hFile::insertRecord(int n){
    int i;

    if (pages.empty()){
        page* pg;
        pg = new page(pageSize);
        pages.push_back(pg);
        pg->insertRecordIntoPage(n);
        return true;
    }

    for(i=0;i<pages.size();i++){
        if (!(pages[i]->isFull())){
            break;
        }
    }

    if (i==pages.size()){
        // insert new page and insert record in it
        page* pg;
        pg = new page(pageSize);
        pages.push_back(pg);
        pg->insertRecordIntoPage(n);
    }
    else{
        // insert into current page pages[i]
        pages[i]->insertRecordIntoPage(n);
    }
    return true;
}

void hFile::externalSort(){
    // firstly sort every page internally
    int i,j,k;
    for(i=0;i<pages.size();i++){
        pages[i]->sortPage();
    }

    cout<<"pages internally sorted seperately\n";
    cout<<"number of pages is "<<pages.size()<<'\n';

    vector <page*> runs; // stores all the runs
    vector <page*> temp;
    vector <int> extraInts;
    int numPages = pages.size(); // denotes number of pages
    int runSize=pageSize; //denotes number of pages in each run
    int numRuns = numPages/bSize; // denotes number of runs
    int bSize2 = bSize-1;

    // for the first iteration - take bunch of b pages and sort everything in them
    i=0;
    while(i<numPages){
        for(j=i;j<min(i+bSize,numPages);j++){
            pages[j]->copyContents(extraInts);
        }
        sort(extraInts.begin(),extraInts.end());

        page* x;
        x = new page(extraInts.size());
        for(k=0;k<extraInts.size();k++){
            x->insertRecordIntoPage(extraInts[k]);
        }
        runs.push_back(x);
        i=i+bSize;
        extraInts.clear();
    }

    cout<<"first iteration done\n";
    cout<<"after first iteration, size of runs is "<<numRuns<<'\n';

    // now runs contains runs of size (extraInts.size() = size of b pages together) which are sorted

    // for every next iteration

    while(numRuns>0){
        // now i have to combine and sort b-1 runs
        i=0;
        numRuns=runs.size();
        cout<<"initially number of runs is "<<numRuns<<'\n';
        while(i<runs.size()){
            for(j=i;j<min(i+bSize2,numRuns);j++){
                runs[j]->copyContents(extraInts);
            }
            sort(extraInts.begin(),extraInts.end());
            cout<<"combined contents of b-1 runs and sorted them\n";

            page* x;
            x = new page(extraInts.size());
            for(k=0;k<extraInts.size();k++){
                x->insertRecordIntoPage(extraInts[k]);
            }
            temp.push_back(x);
            cout<<"inserted the new run into temp\n";
            i=i+bSize2;
            extraInts.clear();
        }
        // now temp has all the runs transfer them to runs vector and empty temp
        runs.clear();
        runs=temp;
        cout<<"size of runs vector is "<<numRuns<<'\n';
        temp.clear();
        cout<<"transferred from temp to runs\n";
        numRuns=runs.size();
        if (numRuns==1){
            break;
        }
    }

    page* y = runs[0];
    y->printPage();

}

void hFile::printSortedFile(){
}

void hFile::printHFile(){
    int i;
    for(i=0;i<pages.size();i++){
        cout<<"page no. "<<i<<" -> ";
        pages[i]->printPage();
        cout<<'\n';
    }
}

hFile::~hFile(){
    //dtor
}

/////////////////////////////////////////////////////////////////////////////

int main(){

    int b,p,d;
    cin>>b>>p>>d;

    hFile* h;
    h = new hFile(b,p);

    int i,j;
    for(i=0;i<d;i++){
        cin>>j;
        h->insertRecord(j);
    }

    h->printHFile();

    h->externalSort();

    return 0;
}
