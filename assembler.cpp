#include<iostream>
#include<string>
#include<string.h>
#include<vector>
#include<unordered_map>
#include<fstream>
#include <sstream>
#include <algorithm>
#include <bitset>
using namespace std ;
struct Labelnode {
	int add ;
	Labelnode* next ;
} ; // Labelnode

struct Node {
	string name ;
	int value ;
	int address ;
} ; // memnode

struct InsNode{
	string name ;
        int opcode ;	
	int operand ;
	int format ;	
	int isXE ;
} ; // InsNode
 
class Hashing {
	public : Node* head ;
	public : Hashing() { 
		head = NULL ;
		head = new Node[100] ;
		for ( int i = 0 ; i < 100 ;i++ ) {
			head[i].address = -1 ;
		} // for	
	} // constractor
	
	public : ~Hashing() {}
	
	public : int hashkey( string labelname ) {
		// cout << "name: " << labelname << endl ;
		int sum = 0 ; 
		int index = 0;
                int ch = '\0' ;
                int maxsize = labelname.size() ;
                while ( index < maxsize ) {
                        ch = labelname[index] ;
			// cout << "ch :" << ch << (int)ch << endl ;
                        sum = sum + ch ;
			// cout << "sum :" << sum << endl ;
                        index++ ;
                } // while
               
	        // cout << "key 49: " << sum << " end" << endl ;	
		return sum % 100  ;
	} // hashkey
	public : bool whetherisinlabellist( string labelname ) {
                int key = hashkey( labelname ) ;
		bool isinlabellist = false, stop = false ;
		int index = key ;
		int times = 0 ;
		while ( head[index].name.empty() != true && stop != true ){
			if ( head[index].name == labelname ) {
				isinlabellist = true ;
				stop = true ;
			} // if
			index++ ;
			times++ ;
			index = index % 100 ;
			if ( times == 100 ) {
				stop = true ;
			} // if
		} // while
		 
		return isinlabellist ;
	} // whetherisinlabellist 

	public : int retrieveitemloc( string labelname ) {
		bool stop = false ;
                int correctkey = hashkey( labelname ) ;
                int times = 0 ;
                while ( head[correctkey].name.empty() != true && stop != true ){
                        if ( head[correctkey].name == labelname ) {
                                stop = true ;
                        } // if
                        correctkey++ ;
                        times++ ;
                        correctkey = correctkey % 100 ;
                        if ( times == 100 ) {
                                stop = true ;
                        } // if
                } // while

		if ( times == 100 ) {
			// cout << "fail " << endl ;
			return 0 ;
		} // if
		else {
			correctkey-- ;
                	return correctkey ;
		} // else
	} // retrieveitemloc

	public : bool insertundefinelabel( string labelname, int encounterloc ) { // label behind ins
		int key =  hashkey( labelname ) ;
		// cout << "key 97= " << key << "  " << labelname << "  " << encounterloc << endl ;
		bool isinlabel =  whetherisinlabellist( labelname ) ;
		if ( isinlabel == false ) {
			//insert
			while ( isinlabel != true ) {
				if ( head[key].name.empty() == true ) {		
					head[key].name = labelname ;
					isinlabel = true ;
				} // if	
				else {
					key++ ;
					key = key % 100 ;
				} // else
		 	} // while
		} // else
		 
		 return isinlabel ;
	} // insertundefinelabel

	public : void insertrealloc( string labelname , int correctloc, int value ) { // label before ins 
                int key = hashkey( labelname ) ;
		bool isinlabel =  whetherisinlabellist( labelname ) ;
		cout << "key 119= " << key << "  " << labelname << "  " << endl ;
		if ( isinlabel == true ) { // find correct key and insert  correct addr
			bool isfind = false ;
			while( isfind != true ) {
				if ( head[key].name == labelname ) {
					head[key].address = correctloc ;
					head[key].value = value ;
					isfind = true ;
				} // if
				else {
					key++ ;
					key = key % 100 ;
				} // else	
			} // while
		} //if
		else {
			while ( isinlabel != true ) {
                                if ( head[key].name.empty() == true ) {
                                        head[key].name = labelname ;
					head[key].address = correctloc ;
					head[key].value = value ;
                                        isinlabel = true ;
                                } // if
                                else {
                                        key++ ;
                                        key = key % 100 ;
                                } // else
                        } // while	
		
		} // else
	} // insertrealloc

	public : void undefindlabeladdress( int &memafterins ) {
		int i = 0 ;
		while ( i < 100 ) {
			if ( head[i].name.empty() == false && head[i].address == -1 ) {	
				head[i].address = memafterins ;
				memafterins += 3 ;
				// cout << "def "  << head[i].name << " " << head[i].address << endl ;
			} // if
		       	// if
			i++ ;
		} // while
	} // undefindlabeladdress

	public : void listlabel() {
		int i = 0 ;
       		while ( i < 100 ) {
			if ( head[i].name.empty() == false  ) {
				cout << head[i].name << "  " ;
				cout << head[i].address ;
				cout << endl ;
			} // if 
			i++ ;
		} // while		
	} // listlabel

} ;// Hashing

class Table {

	public : unordered_map< string, int > delimiter ;
	public : unordered_map< string, int > whitespace ;
	public : unordered_map< string, int > registeR ;
	public : vector< InsNode > instruction ;
	public : unordered_map< string, int > pseudo ;

	public : Table() { 
		createtable() ;
	} // constractor 
	public : ~Table() {}
		
	public : bool iswhitespace( string ch ) {
		string temp = "\0" ;
		bool returnvalue = false ;
		
		for ( auto it = whitespace.begin() ; it != whitespace.end() ; it++ ) {
			temp = (*it).first ;
			if ( temp == ch ) {
				returnvalue = true ;
			} // if
		} // for

		return returnvalue ;		 
	} // iswhitespace

	public : bool isinstruction( string str ) {
		string temp = str ;
		for ( int i = 0 ; i < temp.length() ; i++ ) {
			if ( temp[i] >= 97 && temp[i] <=122 ) {// convert capatial
				temp[i] = temp[i] - 32 ;
			} // if
		} // for

		bool returnvalue = false ;

		for ( auto it = 0 ; it != 59 ; it++ ) {
			if ( instruction[it].name == temp ) {
				returnvalue = true ;
				break ;
			} // if
		} // for
		return returnvalue ; 
	} // isinstruction
	
	public : int retrieveinsInfo( string str ) {
		string tempstr = str ;
		for ( int i = 0 ; i < tempstr.length() ; i++ ) {
			if ( tempstr[i] >= 97 && tempstr[i] <=122 ) {
				tempstr[i] = tempstr[i] - 32 ;
			} // if
		} // for
		int temp = 0 ;
		for ( int it = 0 ; it != 59 ; it++ ) {
		 	if ( instruction[it].name == tempstr ) {
				temp = it ;
				it = 58 ;
		 	} // if
		} // for
		 return temp ;
	} // retrieveinsInfo
	
	public : bool isregisteR( string str ) {
		string tempstr = str ;
		for ( int i = 0 ; i < tempstr.length() ; i++ ) {
			if ( tempstr[i] >= 97 && tempstr[i] <= 122 ) {
				tempstr[i] = tempstr[i] - 32 ;
			} // if
		} // for
		string temp = "" ;
                bool returnvalue = false ;
                for ( auto it = registeR.begin() ; it != registeR.end() ; it++ ) {
                        temp = it -> first ;
                        if ( temp == tempstr  ) {
                                returnvalue = true ;
                                break ;
                        } // if
                } // for

                return returnvalue ; 
	} // isregisteR

	public : bool isdelimiter( string str ) {
                string temp = "" ;
                bool returnvalue = false ;
                for ( auto it = delimiter.begin() ; it != delimiter.end() ; it++ ) {
                        temp = it -> first ;
                        if ( temp == str  ) {
                                returnvalue = true ;
                                break ;
                        } // if
                } // for
                return returnvalue ;	 
	} // isdelimiter

        public : bool ispseudo( string str ) {
                string temp = str ;
                for ( int i = 0 ; i < temp.length() ; i++ ) {
                        if ( temp[i] >= 97 && temp[i] <=122 ) {// convert capatial
                                temp[i] = temp[i] - 32 ;
                        } // if
                } // for
                string temp2 = "" ;
                bool returnvalue = false ;
                for ( auto it = pseudo.begin() ; it != pseudo.end() ; it++ ) {
                        temp2 = it -> first ;
                        if ( temp2 == temp  ) {
                                returnvalue = true ;
                                break ;
                        } // if
                } // for

                return returnvalue ;

        } // ispesudo

        public : bool isDecNumber( string token_Data ) {
                for ( int i = 0 ; i < token_Data.length() ; i ++ ) {
                        if ( token_Data.at( i ) < '0' || token_Data.at( i ) > '9' )
                                return false;
                } // for
                return true;
        } // isDecNumber

	
	public : void createtable() {
		delimiter[","] = 1 ;
	    delimiter["+"] = 2 ;
		delimiter["-"] = 3 ;
		delimiter["*"] = 4 ;
		delimiter["/"] = 5 ;
		delimiter[":"] = 6 ;
		delimiter[";"] = 7 ;
		delimiter["?"] = 8 ;
		delimiter["'"] = 9  ;
		delimiter["."] = 10  ;
		delimiter["="] = 11  ;
		delimiter["#"] = 12  ;
		delimiter["@"] = 13  ;

		whitespace[" "] = 1 ;
		whitespace["\t"] = 2 ;
		whitespace["\n"] = 3 ;
		whitespace["\r"] = 4 ;

		registeR["A"] = 0 ;
		registeR["X"] = 1 ;	
		registeR["L"] = 2 ;
		registeR["B"] = 3 ;
		registeR["S"] = 4 ;
		registeR["T"] = 5 ;
		registeR["F"] = 6 ;
		registeR["PC"] = 7 ;
		registeR["SW"] = 8 ;

		InsNode insnode ;
		insnode.name = 1 ;
		insnode.operand = -1 ;
		insnode.format = -1 ;
		insnode.isXE = -1 ;

		for ( int i = 0 ; i < 59 ; i++ )
			instruction.push_back( insnode ) ;

		instruction[0].name = "ADD" ;
		instruction[0].operand = 1 ;
		instruction[0].format = 3 ;
		instruction[0].opcode = 0x81 ;
		instruction[0].isXE = 0 ;
		instruction[1].name = "ADDF" ;
		instruction[1].operand = 1 ;
		instruction[1].format = 3 ;
		instruction[1].opcode =	0x58 ;
		instruction[1].isXE = 1 ;
		instruction[2].name = "ADDR" ;
		instruction[2].operand = 2 ;
		instruction[2].format = 2 ;
		instruction[2].opcode = 0x90 ;
		instruction[2].isXE = 1 ;
		instruction[3].name = "AND" ;
		instruction[3].operand = 1 ;
		instruction[3].format = 3 ;
		instruction[3].opcode = 0x40 ;
		instruction[3].isXE = 0 ;
		instruction[4].name = "CLEAR" ;
		instruction[4].operand = 1 ;
		instruction[4].format = 2 ;
		instruction[4].opcode = 0xB4 ;
		instruction[4].isXE = 1 ;
		instruction[5].name = "COMP" ;
		instruction[5].operand = 1 ;
		instruction[5].format = 3 ;
		instruction[5].opcode = 0x28 ;
		instruction[5].isXE = 0 ;
		instruction[6].name = "COMPF" ;
		instruction[6].operand = 1 ;
		instruction[6].format = 3 ;
		instruction[6].opcode = 0x88 ;
		instruction[6].isXE = 1 ;
		instruction[7].name = "COMPR" ;
		instruction[7].operand = 2 ;
		instruction[7].format = 2 ;
		instruction[7].opcode = 0xA0 ;
		instruction[7].isXE = 1 ;
		instruction[8].name = "DIV" ;
		instruction[8].operand = 1 ;
		instruction[8].format = 3 ;
		instruction[8].opcode = 0x24 ;
		instruction[8].isXE = 0 ;
		instruction[9].name = "DIVF" ;
		instruction[9].operand = 1 ;
		instruction[9].format = 3 ;
		instruction[9].opcode = 0x64 ;
		instruction[9].isXE = 1 ;
		instruction[10].name = "DIVR" ;
		instruction[10].operand = 2 ;
		instruction[10].format = 2 ;
		instruction[10].opcode = 0x9C ;
		instruction[10].isXE = 1 ;
		instruction[11].name = "FIX" ;
		instruction[11].operand = 0 ;
		instruction[11].format = 1 ;
		instruction[11].opcode = 0xC4 ;
		instruction[11].isXE = 1 ;
		instruction[12].name = "FLOAT" ;
		instruction[12].operand = 0 ;
		instruction[12].format = 1 ;
		instruction[12].opcode = 0xC0 ;
		instruction[12].isXE = 1 ;
		instruction[13].name = "HIO" ;
		instruction[13].operand = 0 ;
		instruction[13].format = 1 ;
		instruction[13].opcode = 0xF4 ;
		instruction[13].isXE = 1 ;
		instruction[14].name = "J" ;
		instruction[14].operand = 1 ;
		instruction[14].format = 3 ;
		instruction[14].opcode = 0x3C ;
		instruction[14].isXE = 0 ;
		instruction[15].name = "JEQ" ;
		instruction[15].operand = 1 ;
		instruction[15].format = 3 ;
		instruction[15].opcode = 0x30 ;
		instruction[15].isXE = 0 ;
		instruction[16].name = "JGT" ;
		instruction[16].operand = 1 ;
		instruction[16].format = 3 ;
		instruction[16].opcode = 0x34 ;
		instruction[16].isXE = 0 ;
		instruction[17].name = "JLT" ;
		instruction[17].operand = 1 ;
		instruction[17].format = 3 ;
		instruction[17].opcode = 0x38 ;
		instruction[17].isXE = 0 ;
		instruction[18].name = "JSUB" ;
		instruction[18].operand = 1 ;
		instruction[18].format = 3 ;
		instruction[18].opcode = 0x48 ;
		instruction[18].isXE = 0 ;
		instruction[19].name = "LDA" ;
		instruction[19].operand = 1 ;
		instruction[19].format = 3 ;
		instruction[19].opcode = 0x00 ;
		instruction[19].isXE = 0 ;
		instruction[20].name = "LDB" ;
		instruction[20].operand = 1 ;
		instruction[20].format = 3 ;
		instruction[20].opcode = 0x68 ;
		instruction[20].isXE = 1 ;
		instruction[21].name = "LDCH" ;
		instruction[21].operand = 1 ;
		instruction[21].format = 3 ;
		instruction[21].opcode = 0x50 ;
		instruction[21].isXE = 0 ;
		instruction[22].name = "LDF" ;
		instruction[22].operand = 1 ;
		instruction[22].format = 3 ;
		instruction[22].opcode = 0x70 ;
		instruction[22].isXE = 1 ;
		instruction[23].name = "LDL" ;
		instruction[23].operand = 1 ;
		instruction[23].format = 3 ;
		instruction[23].opcode = 0x08 ;
		instruction[23].isXE = 0 ;
		instruction[24].name = "LDS" ;
		instruction[24].operand = 1 ;
		instruction[24].format = 3 ;
		instruction[24].opcode = 0x6C ;
		instruction[24].isXE = 1 ;
		instruction[25].name = "LDT" ;
		instruction[25].operand = 1 ;
		instruction[25].format = 3 ;
		instruction[25].opcode = 0x74 ;
		instruction[25].isXE = 1 ;
		instruction[26].name = "LDX" ;	
		instruction[26].operand = 1 ;
		instruction[26].format = 3 ;
		instruction[26].opcode = 0x04 ;
		instruction[26].isXE = 0 ;
		instruction[27].name = "LPS" ;
		instruction[27].operand = 1 ;
		instruction[27].format = 3 ;
		instruction[27].opcode = 0xD0 ;
		instruction[27].isXE = 1 ;
		instruction[28].name = "MUL" ;
		instruction[28].operand = 1 ;
		instruction[28].format = 3 ;
		instruction[28].opcode = 0x20 ;
		instruction[28].isXE = 0 ;
		instruction[29].name = "MULF" ;
		instruction[29].operand = 1 ;
		instruction[29].format = 3 ;
		instruction[29].opcode = 0x60 ;
		instruction[29].isXE = 1 ;
		instruction[30].name = "MULR" ;	
		instruction[30].operand = 2 ;
		instruction[30].format = 2 ;
		instruction[30].opcode = 0x98 ;
		instruction[30].isXE = 1 ;
		instruction[31].name = "NORM" ;
		instruction[31].operand = 0 ;
		instruction[31].format = 1 ;
		instruction[31].opcode = 0xC8 ;
		instruction[31].isXE = 1 ;
		instruction[32].name = "OR" ;	
		instruction[32].operand = 1 ;
		instruction[32].format = 3 ;
		instruction[32].opcode = 0x44 ;
		instruction[32].isXE = 0 ;
		instruction[33].name = "RD" ;	
		instruction[33].operand = 1 ;
		instruction[33].format = 3 ;
		instruction[33].opcode = 0xD8 ;
		instruction[33].isXE = 0 ;
		instruction[34].name = "RMO" ;
		instruction[34].operand = 2 ;
		instruction[34].format = 2 ;
		instruction[34].opcode = 0xAC ;
		instruction[34].isXE = 1 ;
		instruction[35].name = "RSUB" ;
		instruction[35].operand = 0 ;
		instruction[35].format = 3 ;
		instruction[35].opcode = 0x4C ;
		instruction[35].isXE = 0 ;
		instruction[36].name = "SHIFTL" ;
		instruction[36].operand = 2 ;
		instruction[36].format = 2 ;
		instruction[36].opcode = 0xA4 ;
		instruction[36].isXE = 1 ;
		instruction[37].name = "SHIFTR" ;
		instruction[37].operand = 2 ;
		instruction[37].format = 2 ;
		instruction[37].opcode = 0xA8 ;
		instruction[37].isXE = 1 ;
		instruction[38].name = "SIO" ;
		instruction[38].operand = 0 ;
		instruction[38].format = 1 ;
		instruction[38].opcode = 0xF0 ;
		instruction[38].isXE = 1 ;
		instruction[39].name = "SSK" ;
		instruction[39].operand = 1 ;
		instruction[39].format = 3 ;
		instruction[39].opcode = 0xEC ;
		instruction[39].isXE = 1 ;
		instruction[40].name = "STA" ;
		instruction[40].operand = 1 ;
		instruction[40].format = 3 ;
		instruction[40].opcode = 0x0C ;
		instruction[40].isXE = 0 ;
		instruction[41].name = "STB" ;
		instruction[41].operand = 1 ;
		instruction[41].format = 3 ;
		instruction[41].opcode = 0x78 ;
		instruction[41].isXE = 1 ;
		instruction[42].name = "STCH" ;
		instruction[42].operand = 1 ;
		instruction[42].format = 3 ;
		instruction[42].opcode = 0x54 ;
		instruction[42].isXE = 0 ;
		instruction[43].name = "STF" ;
		instruction[43].operand = 1 ;
		instruction[43].format = 3 ;
		instruction[43].opcode = 0x80 ;
		instruction[43].isXE = 1 ;
		instruction[44].name = "STI" ;
		instruction[44].operand = 1 ;
		instruction[44].format = 3 ;
		instruction[44].opcode = 0xD4 ;
		instruction[44].isXE = 1 ;
		instruction[45].name = "STL" ;
		instruction[45].operand = 1 ;
		instruction[45].format = 3 ;
		instruction[45].opcode = 0x14 ;
		instruction[45].isXE = 0 ;
		instruction[46].name = "STS" ;
		instruction[46].operand = 1 ;
		instruction[46].format = 3 ;
		instruction[46].opcode = 0x7C ;
		instruction[46].isXE = 1 ;
		instruction[47].name = "STSW" ;
		instruction[47].operand = 1 ;
		instruction[47].format = 3 ;
		instruction[47].opcode = 0xE8 ;
		instruction[47].isXE = 0 ;
		instruction[48].name = "STT" ;
		instruction[48].operand = 1 ;
		instruction[48].format = 3 ;
		instruction[48].opcode = 0x84 ;
		instruction[48].isXE = 1 ;	
		instruction[49].name = "STX" ;
		instruction[49].operand = 1 ;
		instruction[49].format = 3 ;
		instruction[49].opcode = 0x10 ;
		instruction[49].isXE = 0 ;
		instruction[50].name = "SUB" ;
		instruction[50].operand = 1 ;
		instruction[50].format = 3 ;
		instruction[50].opcode = 0x1C ;
		instruction[50].isXE = 0 ;
		instruction[51].name = "SUBF" ;
		instruction[51].operand = 1 ;
		instruction[51].format = 3 ;
		instruction[51].opcode = 0x5C ;
		instruction[51].isXE = 1 ;
		instruction[52].name = "SUBR" ;		
		instruction[52].operand = 2 ;
		instruction[52].format = 2 ;
		instruction[52].opcode = 0x94 ;
		instruction[52].isXE = 1 ;
		instruction[53].name = "SVC" ;
		instruction[53].operand = 1 ;
		instruction[53].format = 2 ;
		instruction[53].opcode = 0xB0 ;
		instruction[53].isXE = 1 ;
		instruction[54].name = "TD" ;
		instruction[54].operand = 1 ;
		instruction[54].format = 3 ;
		instruction[54].opcode = 0xE0 ;
		instruction[54].isXE = 0 ;
		instruction[55].name = "TIO" ;
		instruction[55].operand = 0 ;
		instruction[55].format = 1 ;
		instruction[55].opcode = 0xF8 ;
		instruction[55].isXE = 1 ;
		instruction[56].name = "TIX" ;
		instruction[56].operand = 1 ;
		instruction[56].format = 3 ;
		instruction[56].opcode = 0x2C ;
		instruction[56].isXE = 0 ;
		instruction[57].name = "TIXR" ;
		instruction[57].operand = 1 ;
		instruction[57].format = 2 ;
		instruction[57].opcode = 0xB8 ;
		instruction[57].isXE = 1 ;		
		instruction[58].name = "WD" ;
		instruction[58].operand = 1 ;
		instruction[58].format = 3 ;
		instruction[58].opcode = 0xDC ;
		instruction[58].isXE = 0 ;

		pseudo["START"] = 1 ;
		pseudo["END"] = 2 ;
		pseudo["BYTE"] = 3 ;
		pseudo["WORD"] = 4 ;
		pseudo["RESB"] = 5 ;
		pseudo["RESW"] = 6 ;
		pseudo["EQU"] = 7;
		pseudo["LTORG"] = 8;
		pseudo["BASE"] = 9 ;
	} // createtable
	
} ; // table


class Scantxt {
	public : string bufferstr ;
	public : int memorystartadd ;
	public : int basereg ;
	public : Table table ;
	public : Hashing symbol ; // label list
	public : Hashing literal ; // const
	public : Hashing strinG ; // "string"
	public : fstream inputfile ;
	public : Labelnode* memeachline ; // for record mew for each line
	public : Labelnode* tailofmemeachline ;
	public : Scantxt() { 
		bufferstr = "" ;
		memorystartadd = 0 ;
		basereg = 0 ;
		memeachline = NULL ;
		tailofmemeachline = NULL ;
	} // constructor 
	public :~Scantxt() {}

        public : bool caseforspecialnum( vector<string> tokengroup, int index ) {
                // x'f1' c'eof'
                if ( index < ( tokengroup.size() - 1 ) ) {
                        if ( tokengroup[index] == "C" && tokengroup[index + 1] == "'" ) {// next -> strinG
                                // index = index + 2 ;
                                return true ;
                        } // else if
                        else if ( tokengroup[index] == "c" && tokengroup[index + 1] == "'" ) {//next -> stingG
                                // index = index + 2 ;
                                return true ;
                        } // else if
                        else if ( tokengroup[index] == "X" && tokengroup[index + 1] == "\'" ) {// next -> literal
                                // index = index + 2 ;
                                return true ;
                        } // else if
                        else if ( tokengroup[index] == "x" && tokengroup[index + 1] == "\'" ) { // next -> literal
                                // index = index + 2 ;
                                return true ;
                        } // else if
                 } // if
                 return false ;
        } // caseforspecialnum

        public : int hexadecimaltodecmial( vector<string> tokengroup, int index ) {
                 string hexnum = tokengroup[index] ;
                 int i = 0 ;
                 int sum = 0, value = 0;
                 string ch = "" ;
                 while( i < hexnum.length() ) {
                        ch = hexnum[i] ;
                        if ( ch == "A" || ch == "a" ) {
                                value = 10 ;
                        } // if
                        else if ( ch == "B" || ch == "b" ) {
                                value = 11 ;
                        } // else if
                        else if ( ch == "C" || ch == "c" ) {
                                value = 12 ;
                        } // else if
                        else if ( ch == "D" || ch == "d" ) {
                                value = 13 ;
                        } // else if
                        else if ( ch == "E" || ch == "e" )  {
                                value = 14 ;
                        } // else if
                        else if ( ch == "F" || ch == "f" ) {
                                value = 15 ;
                        } // else if
                        else {
                                value = stoi( ch ) ;
                        } // else
                        sum = sum * 16 + value ;
                        i++ ;
                 } // while

                 return sum ;
        } // hexadecimaltodecmial
		
	public : void caseforlabellist( vector<string> tokengroup, int index, int mem ) {
		if ( index == 0 ) {
			int value = -1 ;
			/*
			if ( index < tokengroup.size() ) {
				if ( ( tokengroup[index + 1] == "WORD" || tokengroup[index + 1] == "word" ) ||
				       ( tokengroup[index + 1] == "BYTE" || tokengroup[index + 1] == "byte" ) ) {
					if ( caseforspecialnum( tokengroup, index + 2 ) == true ) {
						if ( table.isDecNumber( tokengroup[index + 4] ) )
							value = hexadecimaltodecmial( tokengroup, index + 3 ) ;
					} // if
					else {
						value = stoi( tokengroup[index + 2] ) ;
					} // else
				} // if
			} // if
			cout << "value : " << value << endl ;
			*/
			symbol.insertrealloc( tokengroup[index], mem, 0 ) ;	
		} // if
		else {
			symbol.insertundefinelabel( tokengroup[index], mem ) ;
		} // else	 
	} // caseforlabellist
	
	public : void caseforliteral( int mem, vector<string> tokengroup, int &index ) {
		if ( index == 0 ) {
			int value = -1 ;
			/*
			if ( index < tokengroup.size() ) {
				
				if ( ( tokengroup[index + 1] == "WORD" || tokengroup[index + 1] == "word" ) ||
				     ( tokengroup[index + 1] == "BYTE" || tokengroup[index + 1] == "byte" ) ) {
					if ( caseforspecialnum( tokengroup, index + 2 ) ) {
						if ( table.isDecNumber( tokengroup[index + 4] ) )
							value = hexadecimaltodecmial( tokengroup, index + 3 ) ;

					} // if
					else {
						value = stoi( tokengroup[index + 2] ) ;
					} // else 
				} // if
			} // if
			 */
			literal.insertrealloc( tokengroup[index], mem, 0 ) ;
		} // if
		else {
			literal.insertundefinelabel( tokengroup[index], mem ) ;	
		}  // else
	} // caseforliteral
	
	public : void enqueuememeachline( Labelnode* newnode ) {
		newnode -> next = NULL ;
		if ( memeachline == NULL ) {
			memeachline = newnode ;	
			tailofmemeachline = newnode ;		
		} // if
		else {
			tailofmemeachline -> next = newnode ;
			tailofmemeachline = tailofmemeachline -> next ;
		} // else
	} // enqueuememeachline

	public : int dequeuememeachline() {
		if ( memeachline == NULL ) 
			return -1 ;
		else {
			int pc = memeachline -> add ;
			Labelnode* node = memeachline ;	
			memeachline = memeachline -> next ;
			delete node ;
			return pc ;
		} // else 
	} // dequeuememeachline

	public : void caseforpseudo( int &memafterins, vector<string> tokengroup, int &index, Labelnode* memnode ) {
		string pseudoins = tokengroup[index] ;
		bool specialcase = false ;
		int decimalnum = 0 ;
		if ( pseudoins == "START" || pseudoins == "start" ) { //STATR
			index++ ;
			decimalnum = hexadecimaltodecmial( tokengroup, index ) ;
			memorystartadd = decimalnum ;
			index = tokengroup.size() ;
		} // if
		else if ( pseudoins == "BYTE" || pseudoins == "byte" ) {
			index++ ;
			specialcase = caseforspecialnum( tokengroup, index ) ; 
			if ( specialcase == true ) { 
				if ( tokengroup[index] == "c" || tokengroup[index] == "C" ) {
					int length = tokengroup[index + 2].length() ;
					memafterins = memafterins + length ;
				} // if
				else {  // x X
					int length = tokengroup[index + 2].length() ;
					if ( length % 2 == 1 ) {
						length++ ;
					} // if
					memafterins = memafterins + ( length / 2 ) ;
				} // else
			} // if
			else { //byte 1000
			       // cout << "aaaa" << tokengroup[index + 1] << endl ;
				int decnum = stoi( tokengroup[index ] ) ;
				// cout << "aaaa" << tokengroup[index ] << endl ;
				ostringstream ss ;
				ss << hex << decnum ;
				string result = ss.str();
				int length = result.length() ;
				if ( length % 2 == 1 ) {
					length++ ;
				} // if
				memafterins = memafterins + ( length / 2 ) ;
			} // else
			index = tokengroup.size() ;
		} // else if
		else if ( pseudoins == "WORD" || pseudoins == "word" ) { 
			memafterins = memafterins + 3 ;
			index = tokengroup.size() ;
		} //else 	 
		else if ( pseudoins == "RESW" || pseudoins == "resw" ) {
			index++ ;
			decimalnum = stoi( tokengroup[ index ] ) ;
			memafterins = memafterins + decimalnum * 3 ;
			index = tokengroup.size() ;
		} // else if
		else if ( pseudoins == "RESB" || pseudoins == "resb") {
			index++ ;
			decimalnum = stoi( tokengroup[ index ] ) ;
			memafterins = memafterins + ( decimalnum ) ;
			index = tokengroup.size() ;
		} // else if
		else if ( pseudoins == "EQU" || pseudoins == "equ" ) {   
			if ( table.isDecNumber( tokengroup[index + 1] ) ) {
				cout << "aaaaaa" << endl ;
				memnode -> add = hexadecimaltodecmial( tokengroup , index + 1 ) ;
			} // if
			index = tokengroup.size() ;
		} // else id  
		else if ( pseudoins == "LTORG" || pseudoins == "ltorg" ) {	
			index = tokengroup.size() ;
			literal.undefindlabeladdress( memafterins ) ;
		} // else if
		else if ( pseudoins == "BASE" || pseudoins == "base" ) {
			if ( table.isDecNumber( tokengroup[index + 1] ) ) {
				basereg = hexadecimaltodecmial( tokengroup , index + 1 ) ;
			} // if	
		} // else if
		else {  // end
			literal.undefindlabeladdress( memafterins ) ;
		} // else if
	} // caseforpseudo
	
	public : void caseforins( int &mem, vector<string> tokengroup, int index, int mode ) {
		if ( mode == 0 ) {
			mem = mem + 3 ;
		} //if 
		else { // sic/xe
			int inskey = table.retrieveinsInfo( tokengroup[index] ) ;
			if ( table.instruction[inskey].format == 1 ) { //+j
				mem = mem + 1 ;
				cout << "xe format1" << endl ; 
			} // if
			else if ( table.instruction[inskey].format == 2 ){
				mem = mem + 2 ;
				cout << "xe format2" << endl ;
			} // else if
			else { //table.instruction[inskey].format == 3
				if ( index > 0  && tokengroup[index - 1 ] == "+" )
					mem = mem + 4 ;
				else
					mem = mem + 3 ;
				cout << "xe format3 4" << endl ;
			} // else
		} // else
	} // caseforins

	public : void casefordelimiter( vector<string> tokengroup, int &index ) {
		if ( tokengroup[index] == "#" ) {
			index = tokengroup.size() - 1  ; // immediate value
		} // if
		else if ( tokengroup[index] == "." ) {
			index = tokengroup.size() - 1 ; // 
		} // else if
		
	} // casefordelimiter
	
	public : bool iswhitespacechar( char ch ) {
		if ( ch == ' ' || ch == '\t'  ) {
			return true ;
		} // if
		else
			return false ;
	} // iswhitespacechar

	public : bool isdelimiterchar( char ch ) {
		if ( ch == ',' || ch == '-' || ch == '*' || ch == '/'  || ch == '+' || 
	       	     ch == ':' || ch == ';' || ch == '?' || ch == '\'' ||
		     ch == '.' || ch == '=' || ch == '#' || ch == '@' )
		 	return true ;
		else
			return false ;
	} // isdelimiterchar

	public : bool isnewlinechar( char ch ) {
		if ( ch == '\n' || ch == '\r' )
			return true ;
		else 
			return false ;
	} // isnewlinechar	 
	
	public : void gettokengroup( char &curch, vector<string> &tokengroup ) { 
		string buffer = "" ;
		inputfile.get( curch ) ;
		// cout << "starchar :" << curch << "end" << endl ;
	        while ( !inputfile.eof() && isnewlinechar( curch ) != true ) {
			while ( !inputfile.eof() && iswhitespacechar( curch ) == true ) {
				inputfile.get( curch ) ;
			} // while
	     		while( !inputfile.eof() && iswhitespacechar( curch ) != true 
				&& isdelimiterchar( curch ) != true && isnewlinechar( curch ) != true ) {
				buffer = buffer + curch ;
				inputfile.get( curch ) ;
			} // while	
			if ( !buffer.empty() ) {
				tokengroup.push_back( buffer ) ;
			} // if
			if ( isdelimiterchar( curch ) == true ) { // two token
				buffer = "" ;
				buffer += curch ;
				tokengroup.push_back( buffer ) ;
				inputfile.get( curch ) ;
			} // if
			buffer = "" ;
		} // while		
	} // gettokengroup
	
	public : bool checktokengroup( vector<string> tokengroup ) { // lexical syntx check
		if ( tokengroup.size() != 0 ) {
		/*
			if ( tokengroup[0] == "." ) {
				return false ;
			} // if
		
			int index = 0 ;
			while ( index < tokengroup.size() ) {
				if ( table.isinstruction( tokengroup[index] ) == true ) {
					
				
				} // if
			
			
				index ++ ;
			} // while
			*/
			return true ;
		} // if
		else {
			return false ;	
		} // else
	} // checktokengroup
		
	public : void passesone( string inputstr, int mode ) {
		string s1 = ".txt" ;
		string s = inputstr ;
		s += s1 ;
		int memcurrentadd = 0x0 ;
		int memafterins = 0x0 ;
		char curch = '\0' ;
		int index = 0 ;
		vector <string> tokengroup ;
		Labelnode* memnode = NULL ;
		inputfile.open( s, ios:: in ) ;
		if ( inputfile.is_open() ) {
			while ( !inputfile.eof() ) {
				tokengroup.clear() ;
				gettokengroup( curch, tokengroup ) ; // record mem address
				bool lexicalsyntx = checktokengroup( tokengroup ) ;
				if ( lexicalsyntx && tokengroup.size() != 0 ) {
					memcurrentadd = memafterins ;
					memnode = new Labelnode ;
					memnode -> next = NULL ;
					memnode -> add = memcurrentadd ;
					index = 0;			
					while( index < tokengroup.size() ) {	
						cout << "token: " << tokengroup[index] << "end" << endl ;
						if ( table.isinstruction( tokengroup[index] ) == true ) {
							cout << "ins" << endl ;
							caseforins( memafterins, tokengroup, index, mode ) ;
						} // if
						else if ( index +1 < tokengroup .size() && 
								tokengroup[index] == "x" && tokengroup[index +1] == "'" ) {
							index += 2 ;
							caseforliteral( memcurrentadd, tokengroup, index ) ;	
						} // else if
						else if (  index +1 < tokengroup .size() &&
								tokengroup[index] == "X" && tokengroup[index +1] == "'" ) {
							index += 2 ;
							// cout << memcurrentadd << tokengroup[ index ] << "aa" << endl ;
							caseforliteral( memcurrentadd, tokengroup, index ) ;	
						} // else if
						else if ( index +1 < tokengroup .size() &&
								tokengroup[index] == "c" && tokengroup[index +1] == "'" ) {
							index += 2 ;
							caseforliteral( memcurrentadd, tokengroup, index ) ;
						
						} // else if
						else if ( index +1 < tokengroup .size() &&
								tokengroup[index] == "C" && tokengroup[index +1] == "'" ) {
							index += 2 ;
							caseforliteral( memcurrentadd, tokengroup, index ) ;
						
						} // else if
						else if ( table.isDecNumber( tokengroup[index] ) == true ) {
							if ( index != 0 && tokengroup[index - 1 ] != "=" ) { // comp 0 
								cout << "memaddress" << endl ;
							} // if
							else {
								cout << "literal" << endl ;
								caseforliteral( memcurrentadd, tokengroup, index ) ;
							} // else

						} // else if
						else if ( table.ispseudo( tokengroup[index] ) == true ) {
							cout << "pseudo" << endl ;
							caseforpseudo( memafterins, tokengroup, index, memnode ) ;
						} // else if
						else if ( table.isdelimiter( tokengroup[index] ) == true ) {
							cout << "de" << endl ;
							casefordelimiter( tokengroup, index ) ;	
						} // else if
						else if ( table.isregisteR( tokengroup[index] ) == true ) {	
							cout << "reg" << endl ;
						} // else if
						else {  // label write in label list
							cout << "label" << endl ;
							caseforlabellist( tokengroup, index, memcurrentadd ) ;		
						} // else
						index++ ;
					} // while
					enqueuememeachline( memnode ) ;
				} // if
			} // while
			memnode = new Labelnode ;
			memnode -> next = NULL ;
			memnode -> add = memafterins ;
			enqueuememeachline( memnode ) ;
		} // if
		else {
			cout << "error file is not open" << endl ; 
		} // else

		inputfile.close() ; 
	} // passesone	

        public : string zerostuffing( int mode, string& str, int unit ) {
                int buffer = unit - str.length() ;
                string zerobuffer = "" ;
                while ( buffer > 0 ) {
                        zerobuffer += "0" ;
                        buffer-- ;
                } // while

                if ( mode == 1 ) {  // 0 -> 11000 = 011000
                        str = zerobuffer + str ;
                } // if
                else { // 11000 <- 0 = 110000
                        str = str + zerobuffer ;
                } // else
        	return str ;
	} // zerostuffing
	
	public : void forSICmode( vector<string> tokengroup, int index, string &machinecode ) { // index -> ins 
		 int max = tokengroup.size() - 1 ;
		 int findx = index ;
		 int value = 0x0 ;
		 while( findx < tokengroup.size() ) {
		 	if ( tokengroup[findx] == "X" || tokengroup[findx] == "x" )
		 		value = 0x8000 ;
		 	findx++ ;
		 } // while
		 if ( index < max && symbol.whetherisinlabellist( tokengroup[ index + 1 ] ) ) {
		 	int key = symbol.retrieveitemloc( tokengroup[ index + 1 ] ) ;
			int directadd = symbol.head[key].address + memorystartadd + value ;
			ostringstream ss ;
			ss << hex << directadd ;
			machinecode += ss.str() ;
			zerostuffing( 1, machinecode, 6 ) ;
			cout << "machinecode:" << machinecode << endl ;
		 } // else if
		 else {
			zerostuffing( 2, machinecode, 6 ) ; 
			cout << machinecode << endl ;	
		 } // else
		 return ;
	} // forSICmode

	public : string bintohex( string str ) {
		string hexstr = "" ;
		int i = 0 , n = 0 ;
		string fourbit = "" ;
		while ( i + 4*n < str.length() ) {
			fourbit = "" ;
			for ( i = 0 ; i < 4 ; i++ ) {
				// cout << str[ i+ 4*n ] << " " ;
				fourbit += str[ i+ 4*n ] ;
			} // for
			n++ ;
			if ( fourbit == "0000" ) 
				hexstr += "0" ;
			else if ( fourbit == "0001" )
				hexstr += "1" ;
			else if ( fourbit == "0010" )
                                hexstr += "2" ;
			else if ( fourbit == "0011" )
                                hexstr += "3" ;
			else if ( fourbit == "0100" )
                                hexstr += "4" ;
			else if ( fourbit == "0101" )
                                hexstr += "5" ;
			else if ( fourbit == "0110" )
                                hexstr += "6" ;
			else if ( fourbit == "0111" )
                                hexstr += "7" ;
			else if ( fourbit == "1000" )
                                hexstr += "8" ;
			else if ( fourbit == "1001" )
                                hexstr += "9" ;
			else if ( fourbit == "1010" )
                                hexstr += "a" ;
			else if ( fourbit == "1011" )
                                hexstr += "b" ;
			else if ( fourbit == "1100" )
                                hexstr += "c" ;
			else if ( fourbit == "1101" )
                                hexstr += "d" ;
			else if ( fourbit == "1110" )
                                hexstr += "e" ;
			else if ( fourbit == "1111" )
                                hexstr += "f" ;
			i = 0 ;
		} // while		 
	// 	cout << hexstr << endl ;
		return hexstr ;
	} // bintohex

	public : string format3disp( vector<string> tokengroup, int index, string machinebinary, int mode ) {
		int pc = memeachline -> add ;
		int key = 0 ;
		int disp = 0 ;
		if ( mode == 1 ) {
			key = symbol.retrieveitemloc( tokengroup[ index ] ) ;
			// cout << hex << symbol.head[key].address << " " << hex << pc  << endl ;
			disp = symbol.head[key].address - pc ;
		} // if
		else {
			cout << "1198aaaaaaa" << endl ;
			key = literal.retrieveitemloc( tokengroup[ index ] ) ;
			disp = literal.head[key].address - pc ;
		} // else

		// cout << (dec) << disp << endl ;
		if ( disp < 4095 &&  disp > -4095  ) {
		 	machinebinary += "010" ;
		} // if
		else {
			disp = symbol.head[key].address - basereg ;
			// cout << hex << symbol.head[key].address << " " << hex << basereg  << endl ;
			if ( disp < 4095 && disp > -4095 )
				machinebinary += "100" ;
			else
				return "error disp overflow" ;
		} // else
		bitset <12> dispbinary ( disp ) ;
		string dispbin = dispbinary.to_string() ;
		zerostuffing( 1, dispbin, 12 ) ;
		machinebinary += dispbin ;
		return machinebinary ;
	} // format3disp

	public : string type1Indirectaddr( vector<string> tokengroup, int index, int opcode ) {
		string machinecode = "" ;
		int findx = index ;
	        bool isfindx = false ;
		string machinebinary = "" ;
		int format = 3 ;
		if ( index > 0 && tokengroup[index - 1] == "+" ) {
			format = 4 ;
		} // if
                bitset <8> opcodebin ( opcode ) ;
                machinebinary = opcodebin.to_string() ;
		machinebinary.pop_back() ;
		machinebinary.pop_back() ;
		zerostuffing( 1, machinebinary, 6 ) ;
		machinebinary += "100" ;
		if ( format == 3 ) {
			machinebinary = format3disp( tokengroup, index + 2, machinebinary, 1 ) ;
			int machinedec = stoi( machinebinary, nullptr, 2 ) ;
			ostringstream ss ;
			ss << hex << machinedec ;
			machinecode = ss.str() ;
			zerostuffing( 1, machinecode, 6 ) ;
		} // if
		else { // format == 4
			machinebinary += "001" ;
			int key = symbol.retrieveitemloc( tokengroup[index +2] ) ;
			int address = symbol.head[key].address ;
			bitset <20> addbinary ( address ) ;
			string addbin = addbinary.to_string() ;
			machinebinary += addbin ;
			machinecode = bintohex( machinebinary ) ;
		} // else            	
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
                return machinecode ;
	} // type1Indirectaddr		 
	
	public : string type2Immediateaddr( vector<string> tokengroup, int index, int opcode ) {
		string machinecode = "" ;
		int findx = index ;
		bool isfindx = false ;
		string machinebinary = "" ;
		int format = 3 ;
		if ( index > 0 && tokengroup[index - 1] == "+" ) {
			format = 4 ;
		} // if 
                bitset <8> opcodebin ( opcode ) ;
                machinebinary = opcodebin.to_string() ;
		machinebinary.pop_back() ;
		machinebinary.pop_back() ;
		zerostuffing( 1, machinebinary, 6 ) ;
		machinebinary += "010" ;
		if ( format == 3 ) {
			if ( table.isDecNumber( tokengroup[ index + 2 ] ) ) {
				machinebinary += "000" ;
				int decnum = stoi( tokengroup[ index + 2 ] ) ;
                		bitset <12> binarynum ( decnum ) ;
				machinebinary += binarynum.to_string() ;	
			} // if
			else { // symbol.whetherisinlabellist( tokengroup[index +2] )  lda #data lda #data,x			
				machinebinary = format3disp( tokengroup, index + 2, machinebinary, 1 ) ;
			} // else 
			int machinedec = stoi( machinebinary, nullptr, 2 ) ;
			ostringstream ss ;
			machinecode = ss.str() ;
			zerostuffing( 1, machinecode, 6 ) ;
		} // if
		else{ //format4
			machinebinary += "001" ;
			if ( table.isDecNumber( tokengroup[ index + 2 ] ) ) {
				int decnum = stoi( tokengroup[ index + 2 ] ) ;
				bitset<12> binarynum( decnum ) ;
				machinebinary += binarynum.to_string() ;
			} // if
			else {
				int key = symbol.retrieveitemloc( tokengroup[index +2] ) ;
				int address = symbol.head[key].address ;
				bitset <20> addbinary ( address ) ;
				string addbin = addbinary.to_string() ;
				machinebinary += addbin ;
			} // else
			machinecode = bintohex( machinebinary ) ;
		} // else
		
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
		cout << "machinecode :" << machinecode << endl ;
		return machinecode ;
	} // type2Immediateaddr	 

	public : string type3SimplyaddrLiteral( vector<string> tokengroup, int index, int opcode ) {
                string machinecode = "" ;
                int findx = index ;
                bool isfindx = false ;
                string machinebinary = "" ;
                int format = 3 ;
                if ( index > 0 && tokengroup[index - 1] == "+" ) {
                        format = 4 ;
                } // if
                while ( findx < tokengroup.size() ) {
                        if ( tokengroup[findx] == "X" || tokengroup[findx] == "x" ) {
				if ( findx > 0 && tokengroup[findx -1] == "," )
                                	isfindx = true ;
			} // if
                        findx ++ ;
                } // while
                bitset <8> opcodebin ( opcode ) ;
                machinebinary = opcodebin.to_string() ;
		machinebinary.pop_back() ;
		machinebinary.pop_back() ;
                zerostuffing( 1, machinebinary, 6 ) ;				 
                machinebinary += "11" ;
                if ( isfindx == true )
                        machinebinary += "1" ;
                else
                        machinebinary += "0" ;
		if ( format == 3 ) {
			if ( table.isDecNumber( tokengroup[ index + 2 ] ) ) { // lda =3277
				machinebinary = format3disp( tokengroup, index + 2, machinebinary, 0 ) ;			
			} // if
		 	else {	// lda = x'f1'
				// cout << "1342" << endl ;
				machinebinary = format3disp( tokengroup, index + 4, machinebinary, 0 ) ;
			} // else 
			int machinedec = stoi( machinebinary, nullptr, 2 ) ;
			ostringstream ss ;
			ss << hex << machinedec ;
			machinecode = ss.str() ;
			zerostuffing( 1, machinecode, 6 ) ;
		} // if
		else { // format4
			machinebinary += "001" ;
			int indexoffset = 0 ;
                        if ( table.isDecNumber( tokengroup[ index + 2 ] ) ) 
				indexoffset = 2 ;
                        else 
				indexoffset = 4 ;
			int key = symbol.retrieveitemloc( tokengroup[ index + indexoffset ] ) ;
                        int address = symbol.head[key].address ;
                        bitset <20> addbinary ( address ) ;
                        string addbin = addbinary.to_string() ;
                        machinebinary += addbin ;
			machinecode = bintohex( machinebinary ) ;
		} // else
               
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
                return machinecode ; 
	} // type3SimplyaddrLiteral 

	public : string typr4SimplyaddrLabel( vector<string> tokengroup, int index, int opcode ) { 
		string machinecode = "" ;
		int findx = index ;
                bool isfindx = false ;
                string machinebinary = "" ;
                int format = 3 ;
                if ( index > 0 && tokengroup[index - 1] == "+" ) {
                        format = 4 ;
                } // if
                while ( findx < tokengroup.size() ) {
                        if ( tokengroup[findx] == "X" || tokengroup[findx] == "x" ) {
                                if ( findx > 0 && tokengroup[findx -1] == "," )
                                        isfindx = true ;
                        } // if
                        findx ++ ;
                } // while

		bitset <8> opcodebin ( opcode ) ;	
		machinebinary = opcodebin.to_string() ;
		machinebinary.pop_back() ;
		machinebinary.pop_back() ;
		zerostuffing( 1, machinecode, 6 ) ;
                machinebinary += "11" ;
                if ( isfindx == true )
                        machinebinary += "1" ;
                else
                        machinebinary += "0" ;

		if ( table.isDecNumber( tokengroup[index + 1] ) && format == 3 ) {
			machinebinary += "000" ;
			int disp = stoi( tokengroup[index + 1] ) ;
			bitset <12> dispbin( disp ) ;
			string dispstr = dispbin.to_string() ; 
			machinebinary += dispstr ;
			machinecode = bintohex( machinebinary ) ;
		} // if
		else if ( table.isDecNumber( tokengroup[index + 1] ) && format == 4 ) {
			machinebinary += "000" ;
			int disp = stoi( tokengroup[index + 1] ) ;
			bitset <20> dispbin( disp ) ;
			string dispstr = dispbin.to_string() ;
			machinebinary += dispstr ;
			machinecode = bintohex( machinebinary ) ;
		
		} // if 
		else if ( format == 3 ) { 
                        machinebinary = format3disp( tokengroup, index + 1, machinebinary, 1 ) ;
			int machinedec = stoi( machinebinary, nullptr, 2 ) ;
			ostringstream ss ;
			ss << hex << machinedec ;
			machinecode = ss.str() ;
			zerostuffing( 1, machinecode, 6 ) ;
                } // if
                else { // format == 4
                        machinebinary += "001" ;
			// cout << machinebinary << endl ;
                        int key = symbol.retrieveitemloc( tokengroup[index +1] ) ;
                        int address = symbol.head[key].address ;
                        bitset <20> addbinary ( address ) ;
                        string addbin = addbinary.to_string() ;
                        machinebinary += addbin ;
		       machinecode = bintohex( machinebinary ) ;	
                } // else

		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
                return machinecode ;
	} // typr4SimplyaddrLabel

	public :string type5NoOperand( vector<string> tokengroup, int index, int opcode ) {
		string machinecode = "" ;
                string machinebinary = "" ;
		bitset<8> opcodebin( opcode ) ;
		machinebinary = opcodebin.to_string();
                int machinedec = stoi( machinebinary, nullptr, 2 ) ;
		ostringstream ss ;
		ss << hex << machinedec ;
		machinecode = ss.str() ;
		zerostuffing( 1, machinecode, 2 ) ;
		reverse( machinecode.begin( ) , machinecode.end( ) ) ;
		cout << "machinecode :" << machinecode << endl ;
		return machinecode ;
	} // type5NoOperand

	public : string type6twoReg( vector<string> tokengroup, int index, int opcode ) {
		string machinecode = "" ;
		string machinebinary = "" ;
                bitset<8> opcodebin( opcode ) ;
                machinebinary = opcodebin.to_string();
		string reg1str = tokengroup[ index + 1 ] ;
		string reg2str = tokengroup[ index + 3 ] ;
		for ( int i = 0 ; i < reg1str.length() ; i++ ) {
			if ( reg1str[i] >= 97 && reg1str[i] <= 122 ) {
				reg1str[i] = reg1str[i] - 32 ;
			} // if
		} // for
		for ( int i = 0 ; i < reg2str.length() ; i++ ) {
                        if ( reg2str[i] >= 97 && reg2str[i] <= 122 ) {
                                reg2str[i] = reg2str[i] - 32 ;
                        } // if
                } // for
		int reg1 = table.registeR[ reg1str ] ;
		int reg2 = table.registeR[ reg2str ] ;
		bitset <4> reg1binary ( reg1 ) ;
		string regbin = reg1binary.to_string() ;
		machinebinary += regbin ;
		bitset <4> reg2binary ( reg2 ) ;
		regbin = reg2binary.to_string() ;
		machinebinary += regbin ;
		int machinedec = stoi( machinebinary, nullptr, 2 ) ;
		ostringstream ss ;
		ss << hex << machinedec ;
		machinecode = ss.str() ;
		zerostuffing( 1, machinecode, 4 ) ;
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
		cout << "machinecode :" << machinecode << endl ;
		return machinecode ;	
	} // type6twoReg

	public : string type7RegandInt( vector<string> tokengroup, int index, int opcode ) {
		string machinecode = "" ;	
		string machinebinary = "" ;
                bitset<8> opcodebin( opcode ) ;
                machinebinary = opcodebin.to_string();
		string reg1str = tokengroup[ index + 1 ] ;
                for ( int i = 0 ; i < reg1str.length() ; i++ ) {
                        if ( reg1str[i] >= 97 && reg1str[i] <= 122 ) {
                                reg1str[i] = reg1str[i] - 32 ;
                        } // if
                } // for
		int reg1 = table.registeR[ reg1str ] ;
		bitset <4> reg1binary ( reg1 ) ;
		string regbin = reg1binary.to_string() ;
		machinebinary += regbin ;
	      	int operandint = stoi( tokengroup[index + 3] ) ;	
		if ( operandint >= 0 && operandint < 0xf ) {
			bitset <4> intbin( operandint ) ;
			string intstr = intbin.to_string() ;
			machinebinary += intstr ;
		} // if
		else {
			return "error int overflow" ;
		} // else
		
		int machinedec = stoi( machinebinary, nullptr, 2 ) ;
                ostringstream ss ;
                ss << hex << machinedec ;
                machinecode = ss.str() ;
                zerostuffing( 1, machinecode, 4 ) ;
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
                return machinecode ;
	} // type7RegandInt

	public : string type8OnlyReg( vector<string> tokengroup, int index, int opcode ) {
                string machinecode = "" ;
                string machinebinary = "" ;
                bitset<8> opcodebin( opcode ) ;
                machinebinary = opcodebin.to_string();
                string reg1str = tokengroup[ index + 1 ] ;
                for ( int i = 0 ; i < reg1str.length() ; i++ ) {
                        if ( reg1str[i] >= 97 && reg1str[i] <= 122 ) {
                                reg1str[i] = reg1str[i] - 32 ;
                        } // if
                } // for
                int reg1 = table.registeR[ reg1str ] ;
                bitset <4> reg1binary ( reg1 ) ;
                string regbin = reg1binary.to_string() ;
                machinebinary += regbin ;
		machinebinary += "0000" ;
                int machinedec = stoi( machinebinary, nullptr, 2 ) ;
                ostringstream ss ;
                ss << hex << machinedec ;
                machinecode = ss.str() ;
                zerostuffing( 1, machinecode, 4 ) ;
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
                return machinecode ;
	} // type8OnlyReg 

	public : string type9Onlyint( vector<string> tokengroup, int index, int opcode ) {
                string machinecode = "" ;
                string machinebinary = "" ;
                bitset<8> opcodebin( opcode ) ;
                machinebinary = opcodebin.to_string();
                zerostuffing( 1, machinebinary, 8 ) ;
		int operandint = stoi( tokengroup[index + 1] ) ;
                bitset <8> intbin( operandint ) ;
                string intstr = intbin.to_string() ;
                machinebinary += intstr ;
                int machinedec = stoi( machinebinary, nullptr, 2 ) ;
                ostringstream ss ;
                ss << hex << machinedec ;
                machinecode = ss.str() ;
                zerostuffing( 1, machinecode, 4 ) ;
		// reverse( machinecode.begin( ) , machinecode.end( ) ) ;
                cout << "machinecode :" << machinecode << endl ;
	} // type9Onlyint

	public : string generatemachinecode( vector<string> tokengroup, int index, int mode ) { // index -> ins
		 string machinecode = "" ;
		 int max = tokengroup.size() ;
		 int inskey = table.retrieveinsInfo( tokengroup[index] ) ;
		 int opcode = table.instruction[inskey].opcode ;
		 int format = table.instruction[inskey].format ;
		 ostringstream ss ;
		 ss << hex << opcode ;
		 machinecode = ss.str() ;

		 if ( mode == 0 ) { // sic
		 	forSICmode( tokengroup, index, machinecode ) ;
		 } // if
		 else {
	if ( index < max - 1 && tokengroup[index + 1] == "@" ) { // indirect
		// cout << "1" << endl ;
		machinecode = type1Indirectaddr( tokengroup, index, opcode ) ;
	} // if
	else if ( index < max - 1 && tokengroup[index + 1] == "#" ) { // immediate
	 	// cout << "2" << endl ;
		machinecode = type2Immediateaddr( tokengroup, index, opcode ) ; 
	} // else if
	else if ( index < max - 1 && tokengroup[ index + 1 ] == "=" ) { // lda =3277 , lda = x'f1' 
	 	// cout << "3" << endl ;
		machinecode = type3SimplyaddrLiteral( tokengroup, index, opcode ) ;
	} // else if
	else if ( index < max - 1 && 
		( symbol.whetherisinlabellist( tokengroup[ index + 1 ] ) || table.isDecNumber( tokengroup[index + 1] ) ) ) { 
		// lda data
	 	// cout << "4" << endl ;
		machinecode = typr4SimplyaddrLabel( tokengroup, index, opcode ) ;
	} // else if
	else if ( format == 1 ) { // fix
		// cout << "5" << endl ;
		machinecode = type5NoOperand( tokengroup, index, opcode ) ;
	} // else if
	else if ( format == 2 && index < (max - 3) &&
			( table.isregisteR( tokengroup[index + 1] ) && table.isregisteR( tokengroup[index + 3] ) ) ) { 
		// r1, r2 
		// cout << "6" << endl ;
		machinecode = type6twoReg( tokengroup, index, opcode ) ;
	} // else if
	else if ( format == 2 && index < (max - 2) && table.isregisteR( tokengroup[index + 1] ) ) { // shift r1, n
		 // cout << "7" << endl ;
		 machinecode = type7RegandInt( tokengroup, index, opcode ) ;
	} // else if
	else if ( format == 2 && index < max && table.isregisteR( tokengroup[index + 1] ) ) { // clear r1
		// cout << "8" << endl ;	 
		machinecode = type8OnlyReg( tokengroup, index, opcode ) ;
	} // else if
	else if ( format == 2 && table.isDecNumber( tokengroup[index + 1] ) ) { // svc n
		// cout << "9" << endl ;
		machinecode = type9Onlyint( tokengroup, index, opcode ) ;
	} // else if
	else {
		cout << "wrong" << endl ; 
		return "wrong" ;
	} // else
 
		 } // else
		return machinecode ;
	} // generatemachinecode
	
	public : int dealforEQU( vector<string> tokengroup, int index ) {
		if ( index == 0 ) {
			return 0 ;
		} // if
		int labelloc = 0 ;
		int sum = 0 ;
		int n = 0 ;
		int key = 0 ;
		while ( (index + 2*( n+1 )) < tokengroup.size() ) {
			if ( symbol.whetherisinlabellist( tokengroup[index + 1 + 2*n ] ) ) {
				key = symbol.retrieveitemloc( tokengroup[index + 1 + 2*n] ) ;
				labelloc = symbol.head[key].address ;
			} // if
			else if ( literal.whetherisinlabellist( tokengroup[index + 1 + 2*n ] ) )  { 
				key = literal.retrieveitemloc( tokengroup[index + 1 + 2*n] ) ;
				labelloc = symbol.head[key].address ;
		 	} // else
			else {
				labelloc = stoi( tokengroup[index + 1] ) ;
				return labelloc ;
			} // else
		 	if ( labelloc == - 1 ) {
				cout << "error for EQU calculate" << endl ;
				return 0 ;
			} // if		
			else if ( (index + 1 + 2*n + 1) < tokengroup.size() && tokengroup[index + 1 + 2*n + 1] == "+" ) {
		 		sum = sum + labelloc ;
		 	} // if
		 	else if ( (index + 1 + 2*n + 1) < tokengroup.size() && tokengroup[index + 1 + 2*n + 1] == "-" ) {
				sum = sum - labelloc ;
			} // if
		 	else {
				return 0 ;
			} // else 
			n++ ;
		} // while	
		return sum ;
	} // dealforEQU 


	public : string generateforpseudo( vector<string> tokengroup, int index ) {
		string machinecode = "" ;
		if ( tokengroup[index] == "equ" || tokengroup[index] == "EQU" ) {
			return "" ;
		} // if
		else if ( tokengroup[index] == "byte" || tokengroup[index] == "BYTE" ) { 
			if ( caseforspecialnum( tokengroup, index + 1 ) == true ) {
				if ( tokengroup[ index + 1 ] == "x" || tokengroup[ index + 1 ] == "X" ) {
					return tokengroup[ index + 3 ] ;
				} // if
                                else { // c'eof'
					ostringstream ss ;
                			int k = 0 ;
					while ( k < tokengroup[ index + 3 ].length() ) {
						int chnum = tokengroup[ index + 3 ][k] ;
						ss << hex << chnum ;
						k++ ;
					} // while	
					machinecode += ss.str() ; 
				} // else
                        } // if
                        else {
                        	ostringstream ss ;
				int decnum = stoi( tokengroup[ index + 1 ] ) ;
				ss << hex << decnum ;
				machinecode = ss.str() ;
                        } // else
		} // else if
		else if ( tokengroup[index] == "word" || tokengroup[index] == "WORD" ) {
			if ( caseforspecialnum( tokengroup, index + 1 ) == true ) {
                                if ( tokengroup[ index + 1 ] == "x" || tokengroup[ index + 1 ] == "X" ) {
					machinecode = tokengroup[ index + 3 ] ;
					zerostuffing( 1, machinecode, 6 ) ;
                                } // if
                                else { // c'eof'
                                        ostringstream ss ;
                                        int k = 0 ;
                                        while ( k < tokengroup[ index + 3 ].length() ) {
                                                int chnum = tokengroup[ index + 3 ][k] ;
                                                ss << hex << chnum ;
                                                k++ ;
                                        } // while
                                        machinecode = ss.str() ;
					zerostuffing( 1, machinecode, 6 ) ;
                                } // else
                        } // if
                        else {
                                ostringstream ss ;
				int decnum = stoi( tokengroup[ index + 1 ] ) ;
                                ss << hex << decnum ;
                                machinecode = ss.str() ;
				zerostuffing( 1, machinecode, 6 ) ;
                        } // else
		} // else if
		else if ( tokengroup[index] == "base" || tokengroup[index] == "BASE" ) {
			if ( table.isDecNumber( tokengroup[index + 1] ) ) {
				cout << "1729" << endl ;
				basereg = stoi( tokengroup[index + 1] ) ;
			} // if
			else {
				int key = symbol.retrieveitemloc( tokengroup[ index + 1 ] ) ;
				basereg = symbol.head[key].address ;
			        cout << (dec) << basereg << "aaaa" ; 	
			} // else	
		} // else if
		else {
			return "" ;
		
		} //else
		 return machinecode ;
	} // generateforpseudo
	public : bool notprintmem( string str ) {
		 if ( str == "START" || str == "start" ) {
		 	return true ;
		 } // if
		 else if ( str == "equ" || str == "EQU" ) 
			 return true ;
		 else if ( str == "end" || str == "END" )
			 return true ;
		 else if ( str == "ltorg" || str == "LTORG" )
			 return true ;
		 else if ( str == "BASE" || str == "base" )
			 return true ;
		 else if ( str == "." )
			 return true ;
		 else 
			 return false ;
	} // notprintmem


	public : void passestwo( string inputstr, int mode ) {
		string s1 = ".txt" ;
		string s = inputstr ;
 		s += s1 ;	
		string outstr = "output.txt" ;
		fstream outputfile ;
		vector <string> tokengroup ;
		int index = 0 ;
		char curch = '\0' ;
		string machinecode = "" ;
		inputfile.open( s, ios:: in ) ;
	        outputfile.open( outstr, ios::out ) ;
		int line = 5 ;
		int pc = 0 ;
		outputfile << "Line\t Location\tSource code\t \t Machinecode\n" ;
		outputfile << "---------------------------------------------------\n" ;
		if ( inputfile.is_open() ) { 
			 while ( !inputfile.eof() ) {
                                tokengroup.clear() ;
                                gettokengroup( curch, tokengroup ) ;
				bool lexicalsyntx = checktokengroup( tokengroup ) ;
				if ( tokengroup.size() != 0 && lexicalsyntx ) {
					outputfile << (dec) << line << "\t" ;
					line += 5 ;
					index = 0 ;
					pc = dequeuememeachline() ;
					if ( tokengroup.size() >= 2 && (tokengroup[1] == "EQU" || tokengroup[1] == "equ" )) {
						int equmem = dealforEQU( tokengroup, index ) ;
						outputfile << equmem << "\t" ;
					} // if
					else if ( notprintmem( tokengroup[0] ) == true ) {
						outputfile << "\t\t" ;
					} // else if
					else {
						outputfile << (hex) << pc + memorystartadd << "\t" ;
					} // else


					while ( index < tokengroup.size() ) {
						outputfile << tokengroup[index] << " " ;
						index++ ;
					} // whlile
					outputfile << "\t\t" ;
					index = 0 ;
					while( index < tokengroup.size() ) {
						if ( table.isinstruction( tokengroup[index] ) == true ) {
							cout << tokengroup[index] << "  :" << endl ;
							machinecode = generatemachinecode( tokengroup, index, mode ) ;
						} // if				
						else if ( table.ispseudo( tokengroup[ index ] ) == true ) { 
							cout << tokengroup[index] << "  :" << endl ; 
							machinecode = generateforpseudo( tokengroup, index ) ;			
						} // else if
						else {
							
						} // else if	
						index++ ;
					} // while
					outputfile << machinecode << "\n" ;
				} // if
				machinecode = "" ; // reset
			} // while
		} // if
		else {
			cout << "error file is not open" << endl ;
		} // else
		inputfile.close() ; 
		outputfile.close() ;	 
	} // passestwo	

} ; // Scantxt

int main() {
	Scantxt scantxt ;
	Table table ;	
	cout << "0 for sic 1 for sic/xe : " << endl ;
	int mode = -1 ;
	cin >> mode ;
	string s = "" ;
	cout << "input333.txt cin input333 : " << endl ;
	cin >> s ;
	if ( mode < 2 ) {
		scantxt.passesone( s, mode ) ;
		scantxt.passestwo( s, mode ) ;
	} // if
	else {
		cout << "wrong cin" << endl ;
	} // else
} // main
