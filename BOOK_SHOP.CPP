#include<fstream.h>
#include<stdio.h>
#include<process.h>
#include<conio.h>
#include<string.h>
#include<Math.h>
int l=0, count=3, position;
fstream User;
fstream Account;

// below structure deals with username and password
struct user
{
    char name[40];
    char pass[40];
    char prem;
} obj;

// below class deals with user's account
class account
{
public:
    char name[40], pass[40], prem;
    char books[10][40];
    int quantity[10], n;
    float tprice, price[10];
    account()
    {
        tprice=0.0;
    }
} object;

void options();
void buy();
void changes();
void modify();
void main(); // function prototype declaration

void regster()
{
    clrscr();
    char name[40];
    User.open("user.dat",ios::in|ios::binary);
    cout<<"REGISTRATION FORM \n";
    cout<<"\nEnter a unique user name. (This will be required for signin.)\n";
    gets(name);
    while(User.read(((char*)&obj),sizeof(obj)))
    {
        if(strcmp(obj.name,name)==0)
        {
            cout<<"\nUsername not available.\n";
            User.close();
            getch();
            exit(0);
        }
    }
    User.close();
    cout<<"\nEnter a strong password. (Password should contain minimum of 6 characters.)\n";
    gets(obj.pass);
    if(strlen(obj.pass)<6)
    {
        cout<<"\nEnter correct password.\n";
        getch();
        exit(0);
    }
    char ch;
    cout<<"\nDo you want to be a premium member? Press Y for 'YES' or any character for 'NO'.\n";
    cout<<"You will get flat discount of 10% for a whole year. You will just have to shop regularly.\n";
    cin>>ch;
    if(ch=='Y'||ch=='y')
    {
        obj.prem='Y';
        cout<<"\nYou have been registered successfully as a Premium member.";
    }
    else
    {
        obj.prem='N';
        cout<<"\nYou have been registered successfully as a Non-Premium member.";
    }
    strcpy(obj.name,name);
    User.open("user.dat",ios::out|ios::binary|ios::app);
    User.write(((char*)&obj),sizeof(obj));
    l=1;
    User.close();
    getch();
}

void signin()
{
    clrscr();
    User.open("user.dat",ios::in|ios::binary);
    char name[40],pass[40];
    int k=0;
    cout<<"Enter your user name.\n";
    gets(name);
    cout<<"Enter your password. \n";
    gets(pass);
    while(User.read(((char*)&obj),sizeof(obj)))
    {
        if(strcmp(obj.name,name)==0&&strcmp(obj.pass,pass)==0)
        {
            cout<<"\nLogged in successfully.\n";
            k=1;
            position=User.tellg()-sizeof(obj);
            User.close();
            getch();
            break;
        }
    }
    if(k==0&&count>=1)
    {
        cout<<"\nIncorrect password or username.\n"<<count<<" tries left.\n";
        count--;
        getch();
        signin();
    }
    if(k==0&&count==0)
        exit(0);
}

void changepass()
{
    clrscr();
    cout<<"Enter your new password.\n";
    gets(obj.pass);
    if(strlen(obj.pass)<6)
    {
        cout<<"\nEnter correct password.\n";
        getch();
        changepass();
    }
    else
    {
        User.open("user.dat",ios::out|ios::binary|ios::ate);
        User.seekp(position);
        User.write(((char*)&obj),sizeof(obj));
        cout<<"\nPassword changed successfully. Press enter key to move back.\n";
        User.close();
        getch();
        options();
    }
}

void view()
{
    cout<<"\nS.N. \t NAME \t QUANTITY \t PRICE\n\n";
    for(int i=0; i<object.n; i++)
    {
        cout<<i+1<<" \t "<<object.books[i]<<" \t "<<object.quantity[i]<<" \t "<<object.price[i]<<endl;
    }
    cout<<"\nTOTAL PRICE : Rs. "<<object.tprice;
    if(object.prem=='Y')
    {
        cout<<"\nDiscount Applied (10%) : Rs. "<<(object.tprice/10.0);
        cout<<"\nFINAL PRICE : Rs. "<<(object.tprice-(object.tprice/10.0))<<endl;
    }
}

void cart()
{
    clrscr();
    if(object.n==0)
    {
        cout<<"\nNo item in your cart";
        getch();
        buy();
    }
    else
    {
        view();
        cout<<"\nPress enter key to return back.\n";
        getch();
        changes();
    }
}

void del()
{
    clrscr();
    if(object.n==0)
    {
        cout<<"\nNo item in your cart";
        getch();
        buy();
    }
    else
    {
        int sn;
        view();
        cout<<"\n\nEnter corresponding S.N. of the book which you want to delete.\n";
        cin>>sn;
        if(sn<1||sn>object.n)
        {
            cout<<"\nIncorrect input.\n";
            getch();
            del();
        }
        object.tprice-=(object.price[sn-1]*object.quantity[sn-1]);
        cout<<endl<<object.books[sn-1]<<" has been deleted.\n";
        for(int i=(sn-1); i<object.n; i++)
        {
            strcpy(object.books[i],object.books[i+1]);
            object.quantity[i]=object.quantity[i+1];
            object.price[i]=object.price[i+1];
        }
        object.n--;
        getch();
        modify();
    }
}

void chnge()
{
    clrscr();
    if(object.n==0)
    {
        cout<<"\nNo item in your cart";
        getch();
        buy();
    }
    else
    {
        int qn,temp;
        view();
        cout<<"\n\nEnter corresponding S.N. of the book whose quantity you want to change.\n";
        cin>>qn;
        if(qn<1||qn>object.n)
        {
            cout<<"Incorrect input.\n";
            getch();
            chnge();
        }
        temp=object.quantity[qn-1];
        cout<<"\nEnter new quantity.\n";
        cin>>object.quantity[qn-1];
        if(temp-object.quantity[qn-1]<=0)
            object.tprice+=(object.price[qn-1]*(abs(temp-object.quantity[qn-1])));
        else
            object.tprice-=(object.price[qn-1]*(temp-object.quantity[qn-1]));
        cout<<"\nQuantity has been updated.\n";
        getch();
        modify();
    }
}

void modify()
{
    clrscr();
    int ch;
    cout<<"Press 1 to delete a book.\n";
    cout<<"Press 2 to change the quantity.\n";
    cout<<"Press any other number to return back.\n";
    cin>>ch;
    if(ch==1) del(); //To delete a book
    else if(ch==2) chnge(); //To change the quantity of a book
    else changes(); //Returning back
}

void confirm()
{
    clrscr();
    cout<<"PURCHASE CONFIRMED \n";
    view();
    if(object.prem=='Y')
        object.tprice-=(object.tprice/10.0);
    cout<<"\nTHANKS FOR VISITING !\n";
    Account.open("account.dat",ios::out|ios::binary|ios::app);
    Account.write(((char*)&object),sizeof(object));
    Account.close();
    getch();
    options();
}

void changes()
{
    clrscr();
    int c;
    cout<<"Press 1 to view your cart.\n";
    cout<<"Press 2 to modify.\n";
    cout<<"Press 3 to confirm purchase.\n";
    cout<<"Press any other key to cancel this purchase.\n";
    cin>>c;
    switch(c)
    {
    case 1:
        cart(); //To view cart
        break;
    case 2:
        modify(); //To modify quantity of a book or delete a book
        break;
    case 3:
        confirm(); //To confirm purchase
        break;
    default:
        cout<<"\nCancelling this purchase.";
        getch();
        options(); //Displaying options
    }
}

void buy()
{
    clrscr();
    int x;
    cout<<"List of available books :\n\n";
    cout<<" NAME \t \t \t PRICE \n\n";
    cout<<" 1. Sahitya Sagar \t \t \t Rs. 100.00 \n 2. NCERT Chemistry 12th \t \t \t Rs. 200.00 \n 3. NCERT Physics 12th \t \t \t Rs. 250.00 \n 4. Flamingo \t \t \t Rs. 50.00 \n 5. NCERT Maths 12th \t \t \t Rs. 200.00 \n 6. Sumita Arora \t \t \t Rs. 300.00 \n 7. Sanskrit \t \t \t Rs. 100.00 \n 8. How To Win Friends  \t \t \t Rs. 150.00 \n 9. The Stranger \t \t \t Rs. 250.00 \n 10. Healthy Mind \t \t \t Rs. 400.00 \n";
    cout<<"\nHow many books do you want ?\n";
    cin>>object.n;
    if(object.n>10||object.n<1)
    {
        cout<<"Enter correct quantity.";
        getch();
        buy();
    }
    for(int i=0; i<object.n; i++)
    {
        cout<<"Enter corresponding number.\n";
        cin>>x;
        if(x==1)
        {
            strcpy(object.books[i],"Sahitya Sagar");
            object.price[i]=100.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==2)
        {
            strcpy(object.books[i],"NCERT Chemistry 12th");
            object.price[i]=200.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==3)
        {
            strcpy(object.books[i],"NCERT Physics 12th");
            object.price[i]=250.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==4)
        {
            strcpy(object.books[i],"Flamingo");
            object.price[i]=50.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==5)
        {
            strcpy(object.books[i],"NCERT Maths 12th");
            object.price[i]=200.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==6)
        {
            strcpy(object.books[i],"Sumita Arora");
            object.price[i]=300.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==7)
        {
            strcpy(object.books[i],"Sanskrit");
            object.price[i]=100.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==8)
        {
            strcpy(object.books[i],"How To Win Friends");
            object.price[i]=150.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==9)
        {
            strcpy(object.books[i],"The Stranger");
            object.price[i]=250.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else if(x==10)
        {
            strcpy(object.books[i],"Healthy Mind");
            object.price[i]=400.0;
            cout<<"Quantity?\n";
            cin>>object.quantity[i];
        }
        else
        {
            cout<<"INCORRECT CHOICE.\n";
            object.quantity[i]=0;
            --i;
        }
        object.tprice+=(object.price[i]*object.quantity[i]);
    }
    changes();
}

void last()
{
    clrscr();
    int b;
    if(l==1)
    {
        cout<<"You have just registered. Re-login to view your last purchase.\n";
        getch();
        options();
    }
    else
    {
        account temp;
        Account.open("account.dat",ios::in|ios::binary);
        while(Account.read(((char*)&temp),sizeof(temp)))
        {
            if(strcmp(obj.name,temp.name)==0)
            {
                b=Account.tellg();
            }
        }
        Account.seekg(b-sizeof(temp));
        Account.read(((char*)&temp),sizeof(temp));
        cout<<"Your last purchase :\n\n";
        cout<<"S.N. \t NAME \t QUANTITY \t PRICE\n\n";
        for(int i=0; i<temp.n; i++)
        {
            cout<<i+1<<" \t "<<temp.books[i]<<" \t "<<temp.quantity[i]<<" \t "<<temp.price[i]<<endl;
        }
        cout<<"\n\nTOTAL PRICE : "<<temp.tprice;
        Account.close();
        getch();
        options();
    }
}

void options()
{
    clrscr();
    char choice;
    cout<<"Hi "<<obj.name<<endl<<endl;
    cout<<"Press 1 to change your password.\n";
    cout<<"Press 2 to view your last purchase.\n";
    cout<<"Press 3 to buy new books.\n";
    cout<<"Press 4 to sign out. \n\n";
    cin>>choice;
    switch(choice)
    {
    case '1':
        changepass(); //To change password
        break;
    case '2':
        last(); //To view last purchase
        break;
    case '3':
        buy(); //To buy new books
        break;
    case '4':
        cout<<"\nVISIT AGAIN \n";
        getch();
        exit(0); //Signing out
        break;
    default:
        cout<<"\nIncorrect choice.\n";
        getch();
        options();
    }
}

void main()
{
    clrscr();
    cout<<"WELCOME TO ADARSH'S BOOK SHOP ! \n\n";
    cout<<"If you are new to this shop, Press Y/y else press N/n.\n";
    char ch;
    cin>>ch;
    if(ch=='Y'||ch=='y') regster();
    else if(ch=='N'||ch=='n') signin();
    else
    {
        cout<<"\nPlease enter correct option.";
        exit(0);
    }
    strcpy(object.name,obj.name);
    strcpy(object.pass,obj.pass);
    object.prem=obj.prem;
    options(); //Displaying options
    getch();
}
