//  SyntaxAnalyzer_v3.h
//  Created by Bony Roy(898161054)
//  CPSC 323(3)

#ifndef SyntaxAnalyzer_v3_h
#define syntaxanalyzer_v3_h

#pragma once
#include <string>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <stdlib.h>
#include <stack>

ofstream myfile;





using namespace std;

class SyntaxAnalyzer

{


    int tokenCount;
    string *token;
    string *tokenType;
    int *tokenLineNum;
    int currentIndex;
    int state;
    bool seeSyntax;
    int symbCount;

public:

    // default constructor
    SyntaxAnalyzer()
    {
        seeSyntax =true;
        currentIndex = 0;
        state = 0;
        symbCount = 0;
    }

    // destructor
    ~SyntaxAnalyzer()
    {
        //Deletes allocated memory for the arrays


    }

    void run(string fileName)
    {
        LexicalAnalyzer LR;
        LR.buildLexerfromDatafile(fileName);
        //tokenCount = LR.getNumTokens();
        token = LR.getTokens();
        tokenType = LR.getTokenType();
        tokenLineNum = LR.getTokenLineNum();
        myfile.open("Output.txt");
        if(myfile)
        {
          cout << "\nOutput.txt created in same directory for displaying the Syntax Analyzer Output.\n\n";
        }else{
          	cout << "\nError >> Unable to open output file.\n\n";
            exit(1);
        }

        Rat18F();
        myfile.close();
    }
//Makin function for syntax analyzer

    void Rat18F()
    {

      OptDeclList();
      StatementList();



    }
  //rule OptDeclList
    void OptDeclList(){

      if (token[currentIndex] == "int" || token[currentIndex] == "bool" || token[currentIndex] == "real"||token[currentIndex] == "float")
      {
          DeclList();
      }
      else if(tokenType[currentIndex]=="Identifier"){
          StatementList();
      }
      else if(token[currentIndex] == "if"||token[currentIndex] == "while")
      {

      }else{
          myfile << "ERROR: Expected 'int', 'real' or 'bool' or 'float' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
          exit(1);
      }
    }
//rule Decl list
    void DeclList()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Declaration List> ::= <Declaration>; | <Declaration>;<Declaration List>" << endl;
        }
        Declaration();
        if (token[currentIndex] == ";")
        {
            currentIndex++;

            if (token[currentIndex] == "int" || token[currentIndex] == "bool" || token[currentIndex] == "real"||token[currentIndex] == "float")
            {
                DeclList();
            }
        }
        else
        {
            myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
    }
//rule Declaration
    void Declaration()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Declaration> ::= <Qualifier><IDs>" << endl;
        }

        Qualifier();
        IDs();
    }

    //Rule Qualifier
    void Qualifier()
    {
        if (seeSyntax)
        {
            myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= int | bool | real|float" << endl;
        }

        if (token[currentIndex] == "int") //see what the Qualifier is
        {
            if (seeSyntax)
            {
                myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= int" << endl;
            }
        }
        else if (token[currentIndex] == "bool")
        {
            if (seeSyntax) { myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= bool" << endl; }
        }
        else if (token[currentIndex] == "real")
        {
            if (seeSyntax) { myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= real" << endl; }
        }
        else if (token[currentIndex] == "float")
        {
            if (seeSyntax) { myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Qualifier> ::= float" << endl; }
        }
        else
        {
            myfile << "ERROR: Expected 'int', 'real' or 'bool'or 'float' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
        currentIndex++;

    }

//Rule identifier
    void IDs()
    {
        if (tokenType[currentIndex] == "Identifier")
        {
            if (seeSyntax)
            {
                myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<IDs> ::= <Identifier> | <Identifier>,<IDs>" << endl;
            }



            currentIndex++;
            if (token[currentIndex] == ",")
            {
                currentIndex++;
                IDs();
            }
            else
            {
                Empty();
            }
        }
        else
        {
            myfile << "ERROR: Expected an <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
            exit(1);
        }
     }

//Rule statement
     void StatementList()
     {
         if (seeSyntax)
         {
             myfile << "Token: " << token[currentIndex] <<" Lexeme: " << tokenType[currentIndex] << endl << "\t<Statement List> ::= <Statement> | <Statement> <Statement List>" << endl;
         }
           while (token[currentIndex] == "if" || tokenType[currentIndex] == "Identifier"||  token[currentIndex] == "while")
         {
             Statement();
         }

     }
//Rule statement
     void Statement()
     {
         if (seeSyntax) {
            myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Statement> ::= <Assign> | <If> | <while> " << endl;
        }


         if (tokenType[currentIndex] == "Identifier") {
             Assign();
         }

         else if (token[currentIndex] == "if") {
             If();
         }
         else if (token[currentIndex] == "while") {
             While();
         }

         else {
             myfile << "ERROR: expected <Statement> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
             exit(1);
         }

     }
//assign operator for arithmatic operations
     void Assign()
     {
         if (seeSyntax) {
             myfile << "Token: " << token[currentIndex] <<  " Lexeme: " << tokenType[currentIndex] << endl << "\t<Assign> ::= <Identifier> = <Expression>;\n";
         }

         if (tokenType[currentIndex] == "Identifier") {
             currentIndex++;

             if (token[currentIndex] == "=") {
               if (seeSyntax) {
                   myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl;
               }
                 currentIndex++;
                 Expression();


                 if (token[currentIndex] == ";") {
                     currentIndex++;
                 }

                 else {
                     myfile << "ERROR: Expected ';' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                     exit(1);
                 }
             }

             else {
                 myfile << "ERROR: Expected '=' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                 exit(1);
             }
         }

         else {
             myfile << "ERROR: Expected <identifier> on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
             exit(1);
         }
     }
//Rule if-else
     void If()
     {

         if (token[currentIndex] == "if") {
           if (seeSyntax) {
               myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<If> ::= if ( <Condition> ) { <Statement> } | if ( <Condition> ) {<Statement>} else { <Statement> }\n";
           }

             currentIndex++;
             if (token[currentIndex] == "(") {
                 currentIndex++;
                 Condition();
                 if (token[currentIndex] == ")") {
                     currentIndex++;
                     if(token[currentIndex]=="{"){
                       currentIndex++;
                       Statement();
                       If();
                       if (token[currentIndex] == "}") {
                             currentIndex++;
                         }else {
                             myfile << "ERROR: Expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                             exit(1);
                         }
                      }else {
                         myfile << "ERROR: Expected '{' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                         exit(1);
                      }

                      if (token[currentIndex] == "else") {

                         if (seeSyntax) {
                             myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Else> ::= else { <Statement> }\n";
                         }
                         currentIndex++;
                         if(token[currentIndex]=="{"){

                           currentIndex++;
                           Statement();
                           If();
                         }else {
                             myfile << "ERROR: Expected '{' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                             exit(1);
                         }

                         if (token[currentIndex] == "}") {
                             currentIndex++;
                         }
                         else {
                             myfile << "ERROR: Expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                             exit(1);
                         }
                     }else{
                       myfile << "ERROR: Expected 'else' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                       exit(1);
                     }

                 }
                 else {
                     myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                     exit(1);
                 }
             }
             else {
                 myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                 exit(1);
             }

         }


     }


//Rule condition of if-else
     void Condition()
     {
         if (seeSyntax) {
             myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Condition> ::= <Expression>  <Relop>  <Expression>\n";
         }

         Expression();
         Relop();
         Expression();


     }
     //Rule reloop condition like >, <,=>,<=
          void Relop()
          {
              if (seeSyntax) {
                  myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= ==| > | < | => | =<";
              }

              if (token[currentIndex] == "==") {
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= ==" << endl;
                  }

              }
              else if (token[currentIndex] == "=") {
                currentIndex++;
                if(token[currentIndex]=="="){
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= ==" << endl;
                  }
                }else if(token[currentIndex]==">"){
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= =>" << endl;
                  }
                }else if(token[currentIndex]=="<"){
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= =<" << endl;
                  }
                }
              }

              else if (token[currentIndex] == ">") {
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= >" << endl;
                  }

              }
              else if (token[currentIndex] == "<") {
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] <<  " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= <" << endl;
                  }

              }
              else if (token[currentIndex] == "=>") {
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= =>" << endl;
                  }

              }
              else if (token[currentIndex] == "=<") {
                  if (seeSyntax) {
                      myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Reloop> ::= =<" << endl;
                  }

              }
              else {
                  myfile << "ERROR: Expected '==','>','<','=>','=<' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                  exit(1);
              }



              currentIndex++;
          }
//Rule Expression
     void Expression()
     {
         if (seeSyntax) {
             myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Expression> ::= <Term> <ExpressionPrime>\n";
         }

         Term();
         ExpressionPrime();
     }
//Rule ExpressionPrime for + and -
     void ExpressionPrime()
     {
       if (seeSyntax) {
          myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<ExpressionPrime> ::= + <Term> <ExpressionPrime> | - <Term> <ExpressionPrime> | <Empty>" << endl;
       }

         if (token[currentIndex] == "+") {

             currentIndex++;
             Term();
             ExpressionPrime();
         }
         else if (token[currentIndex] == "-") {

             currentIndex++;
             Term();
             ExpressionPrime();
         }
         else if (tokenType[currentIndex] == "Identifer" || tokenType[currentIndex] == "Keyword")
         {
             myfile << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
             exit(1);
         }

         else {
             if (seeSyntax) {
                 Empty();
              }

         }
     }
  //rules term
     void Term()
     {
         if (seeSyntax) {
             myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Term> ::= <Factor> <TermPrime>" << endl;
         }

         Factor();
         TermPrime();
     }

     void TermPrime()
     {
       if (seeSyntax) {
         myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>" << endl;
       }
         //Analyzer for * and /

         if (token[currentIndex] == "*") {

             currentIndex++;
             Factor();
             TermPrime();
         }
         else if (token[currentIndex] == "/") {

             currentIndex++;
             Factor();
             TermPrime();
         }
         else if (tokenType[currentIndex] == "Identifer" || tokenType[currentIndex] == "Keyword")
         {
             myfile << "ERROR: expected a token on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
             exit(1);
         }
         else {
             if (seeSyntax) {
                 Empty();
              }

         }
     }

    //Analyzer for value after operator
     void Factor()
     {
         if (seeSyntax) {
             myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Factor> ::= <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false" << endl;
         }

         if (tokenType[currentIndex] == "Identifier") {
                currentIndex++;

         }
         else if (tokenType[currentIndex] == "Integer") {
             currentIndex++;
         }
         else if (token[currentIndex] == "(") {
             currentIndex++;
             Expression();

             if (token[currentIndex] == ")") {
                 currentIndex++;

             }
             else {
                 myfile << "ERROR: Expected ')' on line " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                 exit(1);
             }
         }

         else if (tokenType[currentIndex] == "Real") {
             currentIndex++;

         }

         else if (token[currentIndex] == "true") {
             currentIndex++;

         }

         else if (token[currentIndex] == "false") {
             currentIndex++;

         }

         else {
             myfile << "ERROR: missing '<Identifer>', 'Integer', '<Expression>', or '<Qualifier>' on line: " << tokenLineNum[currentIndex] << "Token: " << token[currentIndex] << "Lexeme: " << tokenType[currentIndex];
             exit(1);
         }
     }

///while
void While()
 {
     if (seeSyntax) {
         myfile << "Token: " << token[currentIndex] << endl << "Lexeme: " << tokenType[currentIndex] << endl << "\t<While> ::= while ( <Condition> ){ <Statement> }\n";
     }

     if (token[currentIndex] == "while") {



         currentIndex++;

         if (token[currentIndex] == "(") {
             currentIndex++;
             Condition();
             if (token[currentIndex] == ")") {
                 currentIndex++;


                 if (token[currentIndex] == "{") {
                     currentIndex++;
                     Statement();

                        if (token[currentIndex] == "}") {
                          currentIndex++;
                        }else{
                          myfile << "ERROR: Expected '}' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                          exit(1);
                        }


                 }
                 else
                 {
                     myfile << "ERROR: Expected '{' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                     exit(1);
                 }
             }
             else {
                 myfile << "ERROR: Expected ')' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
                 exit(1);
             }
         }
         else {
             myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
             exit(1);
         }
     }

     else {
         myfile << "ERROR: Expected '(' on line: " << tokenLineNum[currentIndex] << " Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << " ";
         exit(1);
     }
 }


    //empty the token
     void Empty()
     {
         if (seeSyntax) { myfile << "Token: " << token[currentIndex] << " Lexeme: " << tokenType[currentIndex] << endl << "\t<Empty> ::= Epsilon" << endl; }
     }




};

#endif /* SyntaxAnalyzer_v3_h */
