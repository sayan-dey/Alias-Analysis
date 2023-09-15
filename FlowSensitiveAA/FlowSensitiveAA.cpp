#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include <set>

using namespace llvm;
using namespace std;

namespace { //anonymous namespace
class FlowSensitiveAA : public FunctionPass{
public:
    static char ID;
    FlowSensitiveAA(): FunctionPass(ID) {}
    bool runOnFunction(Function &F) override { //for iterating through every function

        set<pair<string,int>> p_set; //set of ptrs where each element is a pair with 1st element=ptr name, 2nd element=ptr level. 
        set<std::string> a_set; //set of all variables

        map<string,set<string>>  mp; //key= a pointer, value= set containing its alias pointers 

        map<string,set<string>>  mp1; //here, key = a pointer, value = set containing non-pointer variables the key is pointing

        auto name = F.getName();
        //errs() <<"Hello from "<<name<<"\n";

        for(Function::iterator bb = F.begin(), e= F.end(); bb!=e; bb++) //for iterating through every basic block of function
        {
            //errs()<<"BasicBlock name ="<< bb->getName()<<"\n";
            //errs()<<"BasicBlock size ="<<bb->size()<<"\n\n";
            
            for(BasicBlock::iterator i = bb->begin(), i2= bb->end(); i!=i2; i++) //for iterating through every line of a basicblock inside the function
            {
                //errs()<<"----"<< *i<<"\n";
                Instruction* inst = dyn_cast<Instruction>(i);


                if(inst->getOpcode() == Instruction::Alloca)
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<s<<"\n";

                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);

                    //errs()<<v1[5][v1[5].size()-2]<<"\n"; //this is for type (eg: i32, or i32*,)
                    
                    /*
                    for(int j=0;j<v1.size();j++)
                    errs()<<v1[j]<<"    ";
                    errs()<<v1.size()<<"\n";
                    */

                    //actually, first 2 elements of v1 are blank
                    
                    string temp=v1[5];
                    temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                    if(temp[temp.size()-1]=='*') //that is, a pointer
                    {
                        int l=count(temp.begin(), temp.end(), '*');
                        p_set.insert(make_pair(v1[2],l));
                
                        //p_set.insert(v1[2]); //variable name (like, %1)
                    }

                }



                if(inst->getOpcode() == Instruction::Load) //modify below code
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<s<<"\n";
                    
                    

                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);

                    //errs()<<v1[5][v1[5].size()-2]<<"\n"; //this is for type (eg: i32, or i32*,)
                    
                    /*
                    for(int j=0;j<v1.size();j++)
                    errs()<<v1[j]<<"    ";
                    errs()<<v1.size()<<"\n";
                    */
                    
                    //errs()<<v1.size()<<"\n";

                    //actually, first 2 elements of v1 are blank

                    string temp=v1[5];
                    temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                    if(temp[temp.size()-1]=='*') //that is, a pointer
                    {
                        int l=count(temp.begin(), temp.end(), '*');
                        p_set.insert(make_pair(v1[2],l));
                        //p_set.insert(v1[2]); //variable name (like, %1)
                    }

                }   //fetching pointers complete

                
                /*
                //Now, this part is for iterations to find aliases (need to put the code in proper place)

                if(inst->getOpcode() == Instruction::Load) 
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<inst->getName()<<"\n";

                    //errs()<<s<<"\n";
                    
                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);

                    if(p_set.count(v1[2]))
                    {
                        if(v1[6][v1[6].size()-2]=='*')
                        {
                            string temp=v1[7];
                            temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                            mp[v1[2]].insert(temp);
                            mp[temp].insert(v1[2]);
                        }
                    }

                }
                */


                if(inst->getOpcode() == Instruction::Load) //modify the code (for points-to analysis ) nned to do for const as well as pointer....not started yet
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<s<<"\n";
                    
                    

                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);



                    //temp=target, temp1=source
                    string temp=v1[2];   
                    
                    int c=0;
                    for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                    {
                        if(itr->first==temp)
                        {
                            c++;
                            break;
                        }
                    }

                    if(c) //as, we need to consider only pointers
                    {
                        string temp1=v1[7];
                        temp1.erase(std::remove(temp1.begin(), temp1.end(), ','), temp1.end());


                        int d=0;
                        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                        {
                            if(itr->first==temp1)
                            {
                                d++;
                                break;
                            }
                        }

                        if((d==0) && (temp1[0]=='%')) //for non-pointers but variables
                        {
                            
                            int l1=0;
                            for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                            {
                                if((itr->first)==temp)
                                l1=itr->second;

                                if(l1>0)
                                break;
                            }

                            if(l1==1)  //temp is 1-ptr (this is the only possible case here as we can't go deep of a non-ptr)
                            mp1[temp].insert(temp1);

                            else//if l1=2
                            {
                                set<string> temp_set=mp1[temp];
                                for(auto itr=temp_set.begin();itr!=temp_set.end();itr++)
                                {
                                    mp1[*itr].clear(); //newly added to kill
                                    mp1[*itr].insert(temp1);
                                }
                            }
                            
                        }   

                        //done
                        //now, write for pointer source
                        //real game begins
                        int l1=0,l2=0;
                         if(d) //pointer source
                         {
                             for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                             {
                                 if((itr->first)==temp)
                                 l1=itr->second;

                                 if((itr->first)==temp1)
                                 l2=itr->second;

                                 if((l1>0) && (l2>0))
                                 break;
                             }

                             if(l1==l2) //done...cool
                             {
                                for(auto itr=mp1[temp1].begin();itr!=mp1[temp1].end();itr++)
                                {
                                    mp1[temp].insert(*itr);
                                }
                             }

                             if(l1==(l2+1))
                             {
                                mp1[temp].clear(); //newly added to kill
                                mp1[temp].insert(temp1);
                             }

                             if(l1>(l2+1))
                             {
                                 set<string> temp_set=mp1[temp];
                                 for(auto itr=temp_set.begin();itr!=temp_set.end();itr++)
                                 {
                                     mp1[*itr].clear(); //newly added to kill
                                     mp1[*itr].insert(temp1);
                                 }
                             }
                         }
                        
                    }
                    
                    /*
                    if(v1[5][v1[5].size()-2]=='*') 
                    {
                        p_set.insert(v1[2]); //variable name (like, %1)
                    }
                    */

                }






                /*
                if(inst->getOpcode() == Instruction::Store) 
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<inst->getName()<<"\n";

                    errs()<<s<<"\n";
                    
                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);

                    errs()<<v1[6]<<"\n";


                    string temp=v1[4];
                    temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                    if(p_set.count(temp)) //as of now, considering only pointers
                    {
                        string temp1=v1[6];
                        temp1.erase(std::remove(temp1.begin(), temp1.end(), ','), temp1.end());
                        mp[temp].insert(temp1);
                        mp[temp1].insert(temp);
                    }

                }
                */


                if(inst->getOpcode() == Instruction::Store)  //modify the code (for pints-to analysis ) need to do for const as well as pointer...done
                {
                    //errs()<<"----"<<*inst<<"\n";
                    std::string s;    // to store alloca instructions
                    llvm::raw_string_ostream(s) << *inst;

                    //errs()<<inst->getName()<<"\n";

                    //errs()<<s<<"\n";

                    
                    vector<string> v1; //v1 = a line (or, instruction) and its each element = a word of the line 
                    string word = "";
                    for (auto x : s) 
                    {
                        if (x == ' ')
                        {
                            v1.push_back(word);
                            word = "";
                        }
                        else {
                            word = word + x;
                        }
                    }
                    if(word.size()>0)
                    v1.push_back(word);

                    //errs()<<v1[6]<<"\n";


                    //temp=source, temp1=target
                    string temp=v1[4];
                    temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                    
                    int d=0;
                    for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                    {
                        if(itr->first==temp)
                        {
                            d++;
                            break;
                        }
                    }

                    if( (d==0) && (temp[0]=='%') ) //that is non-pointer but a variable 
                    {
                        string temp1=v1[6];
                        temp1.erase(std::remove(temp1.begin(), temp1.end(), ','), temp1.end());

                        int c=0;
                        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                        {
                            if(itr->first==temp1)
                            {
                                c++;
                                break;
                            }
                        }


                        //if(p_set.count(temp1)) //we only need to consider pointer here
                        if(c)
                        {
                            int l1=0;
                            for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                            {
                                if((itr->first)==temp1)
                                l1=itr->second;

                                if(l1>0)
                                break;
                            }

                            //continue
                            if(l1==1)
                            mp1[temp1].insert(temp);

                            else//if l1=2
                            {
                                set<string> temp_set=mp1[temp1];
                                for(auto itr=temp_set.begin();itr!=temp_set.end();itr++)
                                {
                                    mp1[*itr].clear(); //newly added to kill
                                    mp1[*itr].insert(temp);
                                }
                            }



                        }   
                        
                    }
                    //done

                    //now , write for pointer
                    //temp=v1[4];
                    //temp.erase(std::remove(temp.begin(), temp.end(), ','), temp.end());
                    if(d) //that is, a pointer
                    {
                        string temp1=v1[6]; 
                        temp1.erase(std::remove(temp1.begin(), temp1.end(), ','), temp1.end());

                        int e=0;
                        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                        {
                            if(itr->first==temp1)
                            {
                                e++;
                                break;
                            }
                        }

                        if(e) //we only need to consider pointer here
                        {
                            int l1=0,l2=0;    
                            for(auto itr=p_set.begin();itr!=p_set.end();itr++)
                             {
                                 if((itr->first)==temp)
                                 l1=itr->second;

                                 if((itr->first)==temp1)
                                 l2=itr->second;

                                 if((l1>0) && (l2>0))
                                 break;
                             }

                             if(l1==l2) //done...cool
                             {
                                for(auto itr=mp1[temp].begin(); itr!=mp1[temp].end();itr++)
                                {   
                                    mp1[temp1].insert(*itr);
                                }
                             }

                             if(l2==(l1+1))
                             {
                                mp1[temp1].clear(); //newly added to kill
                                mp1[temp1].insert(temp);
                             }

                             if(l2>(l1+1))
                             {
                                 set<string> temp_set=mp1[temp1];
                                 for(auto itr=temp_set.begin();itr!=temp_set.end();itr++)
                                 {
                                     mp1[*itr].clear(); //newly added to kill
                                     mp1[*itr].insert(temp);
                                 }
                             }

                            /*
                            for(auto itr=mp1[temp].begin(); itr!=mp1[temp].end();itr++)
                            {
                                mp1[temp1].insert(*itr);
                            }
                            */
                        }
                        /*
                        mp[temp].insert(temp1);
                        mp[temp1].insert(temp);
                        */
                    }

                }



                //converting instruction to string 
                std::string str;
                llvm::raw_string_ostream(str) << *i;
                //errs()<<str<<"\n";
                

                vector<string> v; //v = a line (or, instruction) and its each element = a word of the line 
                string word = "";
                for (auto x : str) 
                {
                    if (x == ' ')
                    {
                        v.push_back(word);
                        word = "";
                    }
                    else {
                        word = word + x;
                    }
                }
                if(word.size()>0)
                v.push_back(word);
                
                /*
                for(int j=0;j<v.size();j++)
                errs()<<v[j]<<" ";
                errs()<<"\n";
                */

                for(int j=0;j<v.size();j++)
                {
                    string str1=v[j];
                    if(v[j][0]=='%')
                    {
                        str1.erase(std::remove(str1.begin(), str1.end(), ','), str1.end());
                        str1.erase(std::remove(str1.begin(), str1.end(), '%'), str1.end());
                    }

                    bool temp=true;
                    for (char const &ch : str1) 
                    {
                        if (std::isdigit(ch) == 0)
                        { 
                            temp= false;
                            break;
                        }
                    }

                    if(temp && v[j][0]=='%')
                    {
                        v[j].erase(std::remove(v[j].begin(), v[j].end(), ','), v[j].end());
                        a_set.insert(v[j]);
                    }
                }


                //errs()<<*i<<"----"<<"\n"; //prints every line (or, instruction...whatever u say)

                
                //int num=i->getNumOperands(); //number of operands in the instruction
                
                /* Not printing req info
                if (i->getNumOperands() >= 1) 
                errs()<<(i->getOperand(0))->getName()<<"\n";
                */
                
                /*  Not printing req info
                for(int j=0;j<num;j++)
                errs()<<(i->getOperand(j))->getName()<<"\n";
                */
                                
                //errs()<<i->getName()<<"\n";
                
                
                /*
                for(int j=0;j<num;j++)
                errs()<<( (i->getOperand(j))->getType()->isPointerTy() )<<"    "; // 1 (if ptr), else 0
                errs()<<i->getName()<<"\n";
                
                errs()<<"\n";
                */


            }

        }


        /*
        errs()<<"Variables: \n\n";
        //errs()<<a_set.size()<<"\n";
        for(auto itr=a_set.begin();itr!=a_set.end();itr++)
        errs()<<*itr<<"\n";
        */

        /*
        errs()<<"Pointer variables: \n\n";
        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
        {
            errs()<<(itr->first)<<"\n";
        }
        */

        //print Point-to table here
        errs()<<"\n\n";
        errs()<<"Points-to information as observed at the last program point in function "<<name<<":"<<"\n";

        for(auto itr=mp1.begin();itr!=mp1.end();itr++)
        {
            errs()<<"Pointer Var "<<(itr->first)<<"={";
            set<string> s_temp=itr->second;
            for(auto itr1=s_temp.begin();itr1!=s_temp.end();itr1++)
            {
                if(itr1!=s_temp.begin())
                errs()<<",";
                errs()<<*itr1;
            }
            
            errs()<<"}\n";
        }


        errs()<<"\n\n";
        //errs()<<"\n"<<"mp.size(): "<< mp.size()<<"\n\n";

        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
        {
            mp[itr->first].insert(itr->first); //as every pointer is alias of itself
        }

        for(auto itr=mp1.begin();itr!=mp1.end();itr++)
        {
            set<string> s_temp = itr->second;
            for(auto itr1=mp1.begin();itr1!=mp1.end();itr1++) //actually, instead of mp1.begin(), we can start from (itr+1)
            {
                set<string> s_temp1=itr1->second;
                for(auto itr2=s_temp.begin();itr2!=s_temp.end();itr2++)
                {
                    for(auto itr3=s_temp1.begin();itr3!=s_temp1.end();itr3++)
                    {
                        if(*itr2==*itr3)
                        {
                            mp[itr->first].insert(itr1->first);
                            mp[itr1->first].insert(itr->first);
                        }

                    }
                }
            }
        }






        /*
        //old code for finding aliases (through transitive relationship)
        for(auto itr=mp.begin();itr!=mp.end();itr++)
        {
            set<string> s_temp=itr->second;
            for(auto itr1=s_temp.begin();itr1!=s_temp.end();itr1++)
            {
                string str1=*itr1;
                set<string> s_temp1=mp[str1];
                for(auto itr2=s_temp1.begin();itr2!=s_temp1.end();itr2++)
                {
                    if(s_temp.count(*itr2)==0)
                    {
                        (itr->second).insert(*itr2);
                        mp[*itr2].insert(itr->first);
                    }
                }
            }
        }
        */


        errs()<<"Alias relationships between the pointers in function "<<name<<":"<<"\n";

        errs()<<"                ";
        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
        {
            errs()<<(itr->first)<<"   ";
        }
        errs()<<"\n";


        for(auto itr=p_set.begin();itr!=p_set.end();itr++)
        {
            errs()<<"Pointer Var "<<(itr->first)<<"  ";
            for(auto itr1=p_set.begin();itr1!=p_set.end();itr1++)
            {
                if((itr->first)==(itr1->first))
                {
                    errs()<<"Yes"<<"  ";
                }
                else if(mp[itr->first].count(itr1->first))
                {
                    errs()<<"Yes"<<"  ";
                }
                else
                {
                    errs()<<"No"<<"  ";
                }
                
            }
            errs()<<"\n";
        }
        errs()<<"\n\n";


        return false;  //we are not doing any transformation (it's an analysis pass)
    
    } //runOnFunction is starting point of any Function Pass

};

}

char FlowSensitiveAA::ID=0;
static RegisterPass<FlowSensitiveAA> res("fsaa", "This is for assg2"); //1st argument will act as opt flag, 2nd argument will get shown for opt --help
