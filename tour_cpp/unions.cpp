
/*
An union is a struct in which all the menbers are allocated at the SAME ADDRESS,
so that the union occupies only as much spaceas its largest member.
Can hold a value for only member at time
*/


/* symbol table entry: name, value*/

unum Type {str,num}:

struct Entry{
  char *name;
  Type t;
  //wasted space for both char or integer value associated with the key name -> UNION
  char *s;  //string value if Type is str
  int i;    //integer value if type is num
}

void f( Entry *p){
  if(p->t == str)
    cout<< p-> s;
}
/**********
* version with UNION ************** */

union Value{
  char *s;
  int i;
}


struct Entry{
  char *key;
  Value v; 
  Type t;    

}

