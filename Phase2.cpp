#include<iostream>
#include<fstream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

//Notations
char M[300][4],IR[4],R[4],C; //Memory,Instruction register,General purpose register,Toggle
int PTR;//page table register
struct PCB
{
    char job[4],TTL[4],TLL[4];
}pcb;
int VA,RA;//virtual and real address
int TTC,LLC,TTL,TLL;
int EM; //error message
int SI,TI,PI;// interrupts
int ttl,tll;

fstream fin,fout;
string line;
int random[30];
int l=-1;// count pages in page table
int IC;
int pte;
int InValid=0;//To check page fault is valid or not(1-not valid,0-valid)

void initialization();
void load();
void Pagetable();
void allocate();
void startExecution();
void executeProgram();
void AddMap();
void Examine();
void MOS();
int Errormsg();
void Terminate();
void read();
void write();

void initialization()
{
    SI=0;TI=0;PI=0;
    TTC=0;LLC=0;TTL=0;TLL=0;
    EM=0; VA=0;RA=0;IC=0;PTR=0;
    InValid=0;
    for(int i=0;i<300;i++)
    {
        for(int j=0;j<4;j++)
        {
            M[i][j]='_';
        }
    }

}

void load()
{
    while(fin)
    {
        int i,j;
        getline(fin,line);

        if(line[0]=='$' && line[1]=='A' && line[2]=='M' && line[3]=='J')
        {
            initialization();
            //Initialize PCB
            for(i=4,j=0;i<8;i++,j++)
            {
                pcb.job[j]=line[i];

            }
            for(i=8,j=0;i<12;i++,j++)
            {
                pcb.TTL[j]=line[i];
            }
            for(i=12,j=0;i<16;i++,j++)
            {
                pcb.TLL[j]=line[i];
            }
            ttl= ((pcb.TTL[0]-48)*1000 + (pcb.TTL[1]-48)*100 + (pcb.TTL[2]-48)*10 + (pcb.TTL[3]-48));
            tll= (pcb.TLL[0]-48)*1000 + (pcb.TLL[1]-48)*100 + (pcb.TLL[2]-48)*10 + (pcb.TLL[3]-48);
            cout<<" TTL = "<<ttl<<endl;
            cout<<" TLL = "<<tll<<endl;

            //initialize Page Table and PTR
            Pagetable();
            //Allocate(get frame for page table)
            allocate();


        }
        if(line[0]=='$' && line[1]=='D' && line[2]=='T' && line[3]=='A')
        {
            startExecution();
        }

    }

}

void Pagetable()
{
    int i,j;
    PTR=(rand()%29)*10;
    cout<<"PTR = "<<PTR<<endl;
    random[PTR/10]=1;

    for(i=PTR;i<PTR+10;i++)
    {
        for(j=0;j<4;j++)
        {
            M[i][j]='*';
        }
    }
}

void allocate()
{
    int level=0;
    int check=0;
    int i,j,pos;
    int k=0,len;
    char str[2];// to store index of block where instruction is load
    while(check!=1)
    {
        pos=(rand()%29)*10;
        cout<<"pos : "<<pos<<endl;
        //l++;
        while(random[pos/10] != 0 )
        {

        pos=(rand()%29)*10;

        }
        random[pos/10]=1;
        itoa(pos,str,10);// convert pos into string

        //M[PTR+l][2]=str[0];
        //M[PTR+l][3]=str[1];
        if(pos<100)
        {
            M[PTR][2]='0';
            M[PTR][3]=str[0];
        }
        else
        {
            M[PTR][2]=str[0];
            M[PTR][3]=str[1];
        }



        getline(fin,line);
        level++;
        k=0;
        for(i=0;i<line.size()/4;i++)
        {
            for(j=0;j<4;j++)
            {
                M[pos+i][j]=line[k];
                k++;
                if(line[k]=='H')
                {
                    check=1;// indicates end of instruction i.e H
                    M[pos+(i+1)][0]='H';
                    M[pos+(i+1)][1]='0';
                    M[pos+(i+1)][2]='0';
                    M[pos+(i+1)][3]='0';
                }

            }
        }
    }

    /*cout<<"\n"<<"MEMORY"<<endl;
    for(i=0;i<300;i++)
    {cout<<"M["<<i<<"] = ";
        for(j=0;j<4;j++)
        {
            cout<<M[i][j];

        }
        cout<<endl;
    }
    */
}

void startExecution()
{
    IC=0;
    executeProgram();
}

void executeProgram()
{
    int no;
    char v[3];
    int i,j,k;
    j=0;
    // finding location of instructions in memory
       // v[0]=M[PTR+l][2];
       //v[1]=M[PTR+l][3];
       v[0]=M[PTR][2];
       v[1]=M[PTR][3];
       v[2]='\0';
       no=((v[0]-48)*10)+(v[1]-48);
      cout<<endl<<"no : "<<no<<endl;
    // loading instructions to IR register
        for(j=0;j<10;j++)
        {
            for(k=0;k<4;k++)
             {
                 IR[k]=M[(no*10)+j][k];
             }
            cout<<"IR : "<<IR<<endl;

            VA=((IR[2]-48)*10)+(IR[3]-48);
            cout<<"VA : "<<VA<<endl;
            AddMap();
            Examine();

        }

}

void AddMap()
{
    int pos;
    char str[2];
    pte=PTR+(VA/10);
    cout<<"pte : "<<pte<<endl;
    if(M[pte][3]== '*')
    {
        PI=3;
        cout<<"Page fault"<<endl;
        pos=(rand()%29)*10;
        while(random[pos/10] != 0 )
        {
        pos=(rand()%29)*10;
        }
        random[pos/10]=1;
        itoa(pos,str,10);
        if(pos/100==0)
        {
            M[pte][2]='0';
            M[pte][3]=str[0];
        }
        else
        {
            M[pte][2]=str[0];
            M[pte][3]=str[1];
        }

    }
    int p=(M[pte][2]-48)*10+(M[pte][3]-48);
    RA=(p*10)+(VA%10);
    cout<<"RA : "<<RA<<endl;
    //if( RA > PTR+10 )
    if( RA > 300 )
    {
        //cout<<"OPERAND ERROR";

        PI=2;
        MOS();
    }

}

void Examine()
{
    if(IR[0]=='G')
    {
         IC=IC+1;
        if(IR[1]=='D')
        {
            cout<<"On GD : \n"<<endl;
            TTC = TTC+2;
            if(TTC <= ttl){
                SI=1;
                MOS();
            }
            else{
                TI=2;
                MOS();
            }
        }
        else
        {
            PI=1;
            MOS();
        }
        cout<<"\nSI = "<<SI<<" TI = "<<TI<<" PI= "<<PI<<endl;
        cout<<"TTC = "<<TTC<<" LLC= "<<LLC<<endl;
    }
    else if(IR[0]=='P')
    {
         IC=IC+1;
        if(IR[1]=='D')
        {
            cout<<"On PD : "<<endl;
            SI=2;
            LLC=LLC+1;
            TTC=TTC+1;
            if(IR[2]=='2'){
                PI=3;
                InValid=1;
                MOS();
            }

            if(LLC <= tll){
                TI=0;
                MOS();

            }

            if(TTC > ttl){
                TI=2;
                MOS();

            }
            else{
                SI=2;
                MOS();
            }
        }
        else
        {
            PI=1;
            MOS();
        }
        cout<<"SI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
        cout<<"\nTTC = "<<TTC<<"  LLC="<<LLC<<endl;

    }
    else if(IR[0]=='H')
    {
        IC=IC+1;
        cout<<"On H :"<<endl;
        TTC=TTC+1;
        SI=3;
        MOS();

    }
    else{
        PI=1;//Operation error
        MOS();
    }

}

void MOS()
{
    if(PI==1)
    {
        if(TI==0){
           EM=4;
           cout<<"Operation Code Error"<<endl;
           fout<<"Operation Code Error"<<endl;
           //Errormsg();
           Terminate();
        }
        else if(TI==2){
            EM=3;
            cout<<"Time Limit Exceeded"<<endl;
            fout<<"Time Limit Exceeded"<<endl;
            //Errormsg();
            EM=4;
            cout<<"Operation Code Error"<<endl;
            fout<<"Operation Code Error"<<endl;
            //Errormsg();
            Terminate();
        }

    }
    else if(PI==2)
    {
        if(TI==0){
           EM=5;
           cout<<"Operand Error"<<endl;
           fout<<"Operand Error"<<endl;
           //Errormsg();
           Terminate();
        }
        else if(TI==2){
             EM=3;
             cout<<"Time Limit Exceeded"<<endl;
             fout<<"Time Limit Exceeded"<<endl;
            //Errormsg();
            EM=5;
            cout<<"Operand Error"<<endl;
            fout<<"Operand Error"<<endl;
            //Errormsg();
            Terminate();
        }

    }
    else if(PI==3)
    {
        if(TI==0){

           //If Page Fault InValid Errormsg(6);
           if(InValid==1){
              EM=6;
              cout<<"Invalid Page Fault"<<endl;
              fout<<"Invalid Page Fault"<<endl;
              //Errormsg();
              Terminate();
            }
        }
        else if(TI==2){
            EM=3;
            cout<<"Time Limit Exceeded"<<endl;
            fout<<"Time Limit Exceeded"<<endl;
            //Errormsg();
            Terminate();
        }

    }
    if(SI==1)
    {
        if(TI==0){
           read();
        }
        if(TI==2){
           EM=3;
           cout<<"Time Limit Exceeded"<<endl;
           fout<<"Time Limit Exceeded"<<endl;
           //Errormsg();
           Terminate();
        }
    }
    if(SI==2)
    {
        if(TI==0){
           write();
        }
        else if(TI==2){
           write();
            EM=3;
            cout<<"Time Limit Exceeded"<<endl;
            fout<<"Time Limit Exceeded"<<endl;
            //Errormsg();
            Terminate();
        }

    }
    if(SI==3)
    {
           EM=0;
           cout<<"No Error"<<endl;
           fout<<"No Error"<<endl;
           //Errormsg();
           Terminate();

    }

}

void Terminate()
{
    fout<<"Job ID : "<<pcb.job[0]<<pcb.job[1]<<pcb.job[2]<<pcb.job[3]<<"  TTL="<<ttl<<"  TLL="<<tll<<endl;
    fout<<"PTR = "<<PTR<<" IR = "<<IR<<" IC = "<<IC<<" EM = "<<EM<<endl;
    fout<<"SI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
    fout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
    cout<<"Job ID : "<<pcb.job[0]<<pcb.job[1]<<pcb.job[2]<<pcb.job[3]<<"  TTL="<<ttl<<"  TLL="<<tll<<endl;
    cout<<"PTR = "<<PTR<<" IR = "<<IR<<" IC = "<<IC<<" EM = "<<EM<<endl;
    cout<<"\nSI = "<<SI<<"  TI ="<<TI<<"  PI="<<PI<<endl;
    cout<<"TTC = "<<TTC<<"  LLC="<<LLC<<endl;
    cout<<""<<endl;
    fout<<"-----------------------------------------------------------------------------------"<<endl;
    fout<<"-----------------------------------------------------------------------------------"<<endl;
    cout<<"\n"<<"MEMORY"<<endl;
    for(int i=0;i<300;i++)
    {cout<<"M["<<i<<"] = ";
        for(int j=0;j<4;j++)
        {
            cout<<M[i][j];

        }
        cout<<endl;
    }
    load();
    exit(0);
}

void read()
{
    cout<<"In Read"<<endl;
    getline(fin,line);
    cout<<line<<endl;
    if(line[0]=='$' && line[1]=='E' && line[2]=='N' && line[3]=='D')
    {
        EM=1;
        cout<<"Out of Data"<<endl;
        fout<<"Out of Data"<<endl;
        Terminate();
    }
    int i,j,k;
    k=0;
    for(i=0; k<=line.size();i++)
    {
        for(j=0;j<4 && k<=line.size();j++)
        {
            M[RA+i][j] = line[k];
            k++;
        }
    }

}

void write()
{
    cout<<"In Write"<<endl;
    char buff[40];
    int ra=0,i,j,k;
    ra=RA;
    k=0;
    if(LLC>tll)
    {
        EM=2;
        cout<<"Line Limit Exceeded"<<endl;
        fout<<"Line Limit Exceeded"<<endl;
        Terminate();
    }
    while(1)
    {
        for(i=0; i<4 ;i++)
        {
            if(M[ra][i] == '_')
                break;
            buff[k] = M[ra][i];
            k++;
        }
        if(M[ra][i] == '_')
                break;
        ra++;
    }
    buff[k]='\0';
    cout<<"\n Line is : "<<buff<<endl;

}

int main()
{
    fin.open("input.txt");
    fout.open("output.txt");
    int i,j;
    for(i=0;i<30;i++)
    {
        random[i]=0;
    }

    load();
    fin.close();
    fout.close();
    return 0;
}

