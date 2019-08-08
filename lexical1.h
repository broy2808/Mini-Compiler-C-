//Bony Roy- 898161054
//Assignment 1
//CPSC 323[Mon-Wed 1:00-2:15pm]
#pragma once
#include <string>
#include <stdexcept>
#include <cctype>
#include <iostream>
#include <iomanip>

using namespace std;

class LexicalAnalyzer
{
    string *keywords;
    string *separators;
    string *operators;
    string *token;
    string *tokenType;
    int lineNum;
    int *tokenLineNum;
    int preSTATE, currentSTATE, index;
public:
    // Basic default constructor
    LexicalAnalyzer()
    {
        currentSTATE = 0;
        preSTATE = 0;
        token = new string[500000];
        tokenType = new string[500000];
        index = 0;
        separators = new string[14] {"{", "}","(",")", "[", "]", ",", "!",".","'",";",":","sp"};
        operators = new string[9] {"-", "+" ,"*", "/","%","=",">","<"};
        keywords = new string[16]  {"int", "float", "bool", "if", "else", "then", "do", "while", "whileend", "do", "doend", "for", "and", "or", "function"};
        lineNum = 1;
        tokenLineNum = new int[1000000];
    }

    // destructor
    ~LexicalAnalyzer()
    {
        //Deletes the dynamically allocated memory
        delete []token;
        delete []tokenType;
        delete []separators;
        delete []operators;

    }


    void updateCurrentState(char s)
    {

         // Previous state update
        preSTATE = currentSTATE;


       // DEFAULT STATE
        if(preSTATE == 0)
        {
          //Comment check
         if(s=='!'){
              currentSTATE = 6;
          }else{

            if(isdigit(s))     //Validate if the entered value is  Number
            {
                addValToLexAna(s);
                currentSTATE = 3;
            }
            if(isalpha(s))     //Validate if the entered value is ALPHA
            {
                addValToLexAna(s);
                currentSTATE = 1;
            }
            if(!isalnum(s))      //Validate everthing except digit or alpha
            {

                // check ' '(Space) and set of default state
                if(s == ' '){
                    currentSTATE = 0;
                }
                else
                {

                    if(isSeparators(s)){//Check if it is a separator
                        addValToLexAna(s);
                        tokenType[index] = "Separator";
                        updateIndex();

                    }else if(isOperators(s)){//Check if it is a operator
                        addValToLexAna(s);
                        tokenType[index] = "Operator";
                        updateIndex();

                    }else if(s=='$'){//Otherwise set it to default state
                      addValToLexAna(s);
                      currentSTATE=7;
                    }else {//Otherwise set it to default state
                      currentSTATE=0;
                    }
                 }
            }
          }
        }
        else if(preSTATE == 1)// Character or Alpha input
        {

            if(isdigit(s)) //Check if it is number then change state.
            {
                addValToLexAna(s);
                currentSTATE = 2;
            }
            if(isalpha(s))//Check if it is alpha then keep in current state
            {
                addValToLexAna(s);
                currentSTATE = 1;
            }
            if(!isalnum(s) and s!='$')//Check if it is not number or alpha or '$' then validate if it is a keyword or not
            {
                //create a token
                if(isValidKeyword(token[index]))
                    tokenType[index] = "Keyword";
                else
                    tokenType[index] = "Identifier";
                currentSTATE = 0;

                if(s == ' '){//If next is ' '(space). Simply update index value
                  updateIndex();
                }
                else//Update index and send the s value to lexer to validate current state
                {   updateIndex();
                    updateCurrentState(s);
                }

            }else if(s=='$'){//This is to validate num1$ identifiers
              addValToLexAna(s);
            }
        }
        else if(preSTATE == 2)  // NUMERIC INPUT AFTER ALPHA
        {
           //alpha input after alphanumeric
            if(isalpha(s))
            {
                currentSTATE = 1;
                updateCurrentState(s);
             }

            if(isdigit(s))// NUMERIC INPUT keep the current state
            {
                addValToLexAna(s);
                currentSTATE = 2;
            }
            if(!isalnum(s) and s!='$')
            {

                currentSTATE = 0;
                if(isValidKeyword(token[index]))
                    tokenType[index] = "Keyword";
                else
                    tokenType[index] = "Identifier";//Digit after alpha is valid identifier
                updateIndex();
                updateCurrentState(s);

            }else if(s=='$'){
              addValToLexAna(s);
            }
        }
        else if(preSTATE == 3)  // NUMERIC INPUT to lexer
        {

            if(isdigit(s))//If digit valid token keep in current state
            {
                addValToLexAna(s);
                currentSTATE = 3;
            }
            if(isalpha(s))//If we enter alpha after numeric, it is not a valid token. Send to state 7
            {
                addValToLexAna(s);
                currentSTATE = 7;
            }
            if(!isalnum(s))//If we enter not alphanumeric value after number
            {
                // check ' ' or functions
                if(s == ' ')//If we enter ' '(space)it is valid
                {
                    tokenType[index] = "Integer";
                    currentSTATE = 0;
                    updateIndex();
                }
                else if (s == '.')//If we enter '.'.It can be a real number. So,change state to 4
                {
                    addValToLexAna(s);
                    currentSTATE = 4;
                }
                else if(isSeparators(s)or isOperators(s))//If next value is separator or operator it is integer
                {
                    tokenType[index] = "Integer";
                    updateIndex();
                    currentSTATE = 0;
                    updateCurrentState(s);
                }else{//Else invalid
                  addValToLexAna(s);
                  currentSTATE = 7;
                }

            }
        }
        else if(preSTATE == 4)//Check if the number is rational number or not
        {
          //If there is alpha send it to invalid token
            if(isalpha(s))
            {
                currentSTATE = 7;
                updateCurrentState(s);
            }
            //If there is number keep in same state
            if(isdigit(s))
            {
                currentSTATE = 4;
                addValToLexAna(s);
            }
            if(!isalnum(s))
            {
                // check ' ' or functions
                tokenType[index] = "Real";
                currentSTATE = 0;
                updateIndex();
                if(s != ' '){
                  updateCurrentState(s);
                }


            }
        }
        //NON Alphanumeric check(Not implemented properly. Will be required to check double operators or separators
        else if(preSTATE == 5)
        {
          if(isalpha(s))
          {
              //create token
              currentSTATE = 1;
              updateIndex();
              updateCurrentState(s);
          }
          if(isdigit(s))
          {
              //create token
              currentSTATE = 3;
              updateIndex();
              updateCurrentState(s);

          }
            if(s=='!'){
              currentSTATE = 6;
            }
            else
            {
               if(!isalnum(s))
              {
                  if (s==' '){
                    currentSTATE=0;
                  }
                  // create token check type
                  else if(isSeparators(s)){
                        tokenType[index] = "Separator";
                        updateIndex();
                  }else if(isOperators(s)){
                        tokenType[index] = "Operator";
                        updateIndex();
                  }
               }
               currentSTATE=0;
            }

        }
        else if(preSTATE == 6)  // Validating comments and removing them
        {
          if(s=='!'){ // As per the syntax comment will start and end with !. It is to check the end
            currentSTATE = 0;
          }
        }

        else if(preSTATE == 7)// Invalid tokens
        {
            if(isalpha(s))//If the next value is alpha, add it to lex and stay on current state.
            {
                addValToLexAna(s);
            }
            if(isdigit(s))//If the next value is number, add it to lex and stay on current state.
            {
                addValToLexAna(s);
            }
            if(!isalnum(s))
            {
                //check alphanumeric inputs
                if(s == ' ')
                {
                    tokenType[index] = "Invalid token";
                    currentSTATE = 0;
                    updateIndex();
                }
                else
                {
                    tokenType[index] = "Invalid token";
                    updateIndex();
                    currentSTATE = 0;
                    updateCurrentState(s);
                }

            }
        }

    }
    //Method to update index
    void updateIndex()
    {
        tokenLineNum[index] = lineNum;
        index++;
    }
     //Method to add value to lexer
    void addValToLexAna(char s)
    {
        token[index] += s;
    }
    //Method to reset the state
    void resetState()
    {
        currentSTATE = 0;
        preSTATE = 0;
    }
    //Method to validate operators
    bool isOperators(char sep)
    {
      string s;
      s+=sep;
      for(int i = 0; i < 9; i++)
      {
          if(s == operators[i]) {return 1;}
      }
      return 0;
    }
    //Method to validate keywords
    bool isValidKeyword(string tokenIndex)
    {

        for(int i = 0; i < 16; i++)
        {
            if (tokenIndex == keywords[i])
            {
              return 1;
            }
        }
        return 0;
    }
    //Method to validate separators
    bool isSeparators(char sep)
    {
        string s;
        s+=sep;
        for(int i = 0; i < 14; i++)
        {
            if(s == separators[i]) {return 1;}
        }
        return 0;
    }



    string *getTokens()
    {
        return token;
    }
    string *getTokenType()
    {
        return tokenType;
    }

    int *getTokenLineNum()
    {
        return tokenLineNum;
    }

    // Load information from a text file with the given filename.
    void buildLexerfromDatafile(string filenamein)
    {
        fstream myfile(filenamein);
        //Open the file
        if (myfile.is_open())
        {
            cout << "File Opened Successfully: " << filenamein << endl;
            string line;
            char s;
            //Read line by line from file
            while (getline(myfile,line))
            {
                line += ' ';
                //Break line into individual characters check state
                for(int i = 0; i < line.length(); i++)
                {
                    s = line[i];
                    updateCurrentState(s);
                }
                lineNum++;
            }
            myfile.close();
            //Print the token and lexems combination in output window
            cout<<"TOKENS       "<<"     LEXEMES     \n"<<endl;
            for(int i = 0; i < index; i++)
            {
            int size1=20-tokenType[i].size();
            cout  << left<<"" <<   tokenType[i];
            cout<<std::setfill(' ') <<right<<std::setw(size1);
            cout<<"=    " << token[i] <<  endl;
            }
            //Writing into a output file
            ofstream myfile("lexer_output_Bony.txt");
            if (myfile.is_open())
             {
               myfile<<"TOKENS       "<<"      LEXEMES     \n"<<endl;
               for(int i = 0; i < index; i++)
               {
               int size1=20-tokenType[i].size();
               myfile<< left<<"" <<   tokenType[i];
               myfile<<std::setfill(' ') <<right<<std::setw(size1);
               myfile<<"=    " << token[i] <<  endl;
               }
               cout<<"Output file : 'lexer_output_Bony.txt' has been created."<<endl;
             }else{ cout << "Unable to open output file";}//Else throw error if unable to open the output file
           }
            else {//Else throw error if unable to open the input file
            throw invalid_argument("Unable to open the file :" + filenamein);
          }
        myfile.close();//Close the file
    }



};
