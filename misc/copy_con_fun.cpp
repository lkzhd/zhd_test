#include <fstream>
#include <string>


using namespace std;
ofstream out("HowMany2.out");

class HowMany{
    string              name;
    static int          objectcount;
public:
    HowMany2(const string& id = ""):name(id){
        ++objectcount;
        print("HowMany2()");
    }
    ~HowMany2(){
        --objectcount;
        print("~HowMany2()");
    }
    /* copy constructor
     * */
    HowMany2(const HowMany2& rhs):name(rhs.name){
        name += " copy";
        ++objectcount;
        print("HowMany2(const HowMany2 &)");
    }
    void print(const string& msg = "")const{
        if(0 != msg.size()){
            out << msg << endl;
        }
        out << "\t" << name << ": "
            << "objectcount = "
            << objectcount << endl;
        
    }
};

int HowMany2::objectcount = 0;
HowMany2 f(HowMany2 x){
    x.print("x argument inside f()");
    out << "Returning from f()" << endl;
    return x;
}


int
main{
    HowMany2 h("h");
    out << "Entering f()" << endl;
    HowMany2 h2 = f(h);
    h2.print("h2 after call to f()");
    out << "call f(),no return value" << endl;
    f(h);
    out << "after call to f()" << endl;
}









