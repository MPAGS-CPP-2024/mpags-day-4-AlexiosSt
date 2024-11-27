#include "PlayfairCipher.hpp"

#include <iostream>
#include <algorithm>
#include <iterator> 
using namespace std;

PlayfairCipher::PlayfairCipher(const std::string& key){
    std::cout<<"You gave me key: "<<key<<std::endl;
    this->setKey(key);
}

void PlayfairCipher::setKey(const std::string& key){
    string amsg{""};

    //store the original key
    key_ = key;

    //Append the alphabet
    const string abc{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    key_+=abc;

    //Make sure the key is uppercase
    std::transform(key_.begin(), key_.end(), key_.begin(), ::toupper);

    //Remove non-alpha characters
    auto iter = std::remove_if( begin(key_), end(key_), 
            [](char c){if (isalpha(c)) return false; else return true;} );

    key_.erase(iter, key_.end());

    //Change J->I
    std::transform(key_.begin(), key_.end(), key_.begin(), 
            [](char c) { if (c=='J') c='I'; return c; } ) ;

     //Remove duplicated letters
    string storech{""};
    auto iter2 = remove_if( begin(key_), end(key_), 
            [&storech](char c){if (storech.find(c)<=storech.size()) {return true;}
                else {storech+=c; return false;} } );
    key_.erase(iter2, key_.end());    
    
    amsg=key_;
    cout<<"Key is now "<<amsg<<" while stored chars is: "<< storech<<endl;
    //Store the coords of each letter
    int x{0}, y{0};
    for (size_t i=0; i<key_.size(); i++){
        if (i!=0 && i%5==0) {x++; y=0;};
        auto p{make_pair(x,y)}; // p: point in the grid, aka coords!
        ch2coord_[key_[i]] = p;
        coord2ch_[p]=key_[i];
        y++;
    }
}

std::string PlayfairCipher::applyCipher(const std::string& inTxt, const CipherMode cMode) const{

    std::string input{"Hello World!! "+inTxt};
    string outTxt{inTxt};  

    // Change J -> I
    transform(begin(outTxt), end(outTxt), begin(outTxt),
            [] (char c){return (c=='J')? 'I' : c;});

    //If repeated chars in a digraph add an X (or Q if XX) between them
    string tmpTxt{""};
    tmpTxt.reserve(outTxt.size()*1.1);
    for (size_t i=0; i<outTxt.size(); i+=2){
        tmpTxt+=outTxt[i];
        if (i + 1 == outTxt.size()) {
            tmpTxt += (outTxt[i] == 'Z') ? 'X' : 'Z';
            break;
        }
        else if (outTxt[i]!=outTxt[i+1]){
            tmpTxt+=outTxt[i+1];
        }
        else{
            tmpTxt+= (outTxt[i]=='X')? 'Q' : 'X';
            --i;
        }
    }

    outTxt.swap(tmpTxt);
    cout<<"Formatted input is "<<outTxt<<endl;

    //LOOP in digraphs: - Find the coords in the grid for each digraph
    //                  - Apply the rules to these coords to get 'new' coords
    //                  - Find the letter associated with the new coords
    IntPair xy1, xy2; int x1, y1, x2, y2; //coords and column/row of each letter in digraph!
    for (size_t i=0; i<outTxt.size(); i+=2){        
        auto iterxy1=ch2coord_.find(outTxt[i]);
        auto iterxy2=ch2coord_.find(outTxt[i+1]);
        if (iterxy1==ch2coord_.end() || iterxy2==ch2coord_.end()) 
            return "[error] Letter not found in the formatted input text!!";
        xy1=iterxy1->second;
        xy2=iterxy2->second;
        x1=xy1.first; y1=xy1.second;
        x2=xy2.first; y2=xy2.second;
        //cout<<typeid(outTxt[0]).name()<<endl; //to see the data type of a letter in outTxt
        //cout<<"xy1: "<<x1<<" "<<y1<<"\nxy2: "<<x2<<" "<<y2<<endl;

        if (x1==x2) {
            if (cMode==CipherMode::Encrypt){
                if (y1!=4) y1++; else y1=0;
                if (y2!=4) y2++; else y2=0;
            }
            else{
                if (y1!=0) y1--; else y1=4;
                if (y2!=0) y2--; else y2=4;
            }
        }
        else if (y1==y2){
            if (cMode==CipherMode::Encrypt){ 
                if (x1!=4) x1++; else x1=0;
                if (x2!=4) x2++; else x2=0;
            }
            else{
                if (x1!=0) x1--; else x1=4;
                if (x2!=0) x2--; else x2=4;
            }
        }      
        else{ //swap the columns of 1 and 2!
            int tmp{0}; 
            tmp=y2;
            y2=y1;
            y1=tmp;            
        }

        xy1={x1, y1}; xy2={x2, y2};
        auto iterch1=coord2ch_.find(xy1);
        auto iterch2=coord2ch_.find(xy2);
        if (iterch1==coord2ch_.end() || iterch2==coord2ch_.end()) 
            return "[error] New coords not found in the grid!!";
        
        outTxt[i]=iterch1->second; outTxt[i+1]=iterch2->second;
    }

    //return the text!!
    return outTxt;
}