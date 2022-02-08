#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 10000

/**********************************************************/
/************** Stack Datatype and Operations *************/
/**********************************************************/

// stack structure declaration
typedef struct stackADT {
    char opr[MAX];
    int top;
} Stack;

// initialize stack
void initStack (Stack *stack)
{
    stack->top = -1;
}

// check whether stack is empty
int isEmptyStack (Stack *stack)
{
    return (stack->top == -1);
}

// check whether stack is full
int isFullStack (Stack *stack)
{
    return (stack->top == MAX-1);
}

// push an element into stack
void push (Stack *stack, char x)
{
    if (isFullStack(stack))
        printf("Error: Stack is Full!\n");
    else
        stack->opr[++(stack->top)] = x;
}

// pop an element from stack
char pop (Stack *stack)
{
    char x;
    if (isEmptyStack(stack))
        printf("Error: Stack is Empty!\n");
    else
        x = stack->opr[(stack->top)--];
    return x;
}

/**********************************************************/


/**********************************************************/
/*********** Binary Tree Datatype and Operations **********/
/**********************************************************/

// binary tree structure declaration
typedef struct binTree {
    char element;
    struct binTree *leftChild;
    struct binTree *rightChild;
} BT;

// creating a node in binary tree
BT *createNode (char el)
{
    BT *newNode;
    if ( (newNode=(BT *)malloc(sizeof(BT))) == NULL )
        printf("Error: Malloc Error in Creating Node!\n");
    else {
        newNode->element = el;
        newNode->leftChild = NULL;
        newNode->rightChild = NULL;
    }
    return (newNode);
}

// creating an exact replica of the the binary tree
BT *duplicate(BT *orig)
{
    BT *dup = NULL;
    if(orig != NULL) {
        dup = createNode(orig->element);
        dup->leftChild = duplicate(orig->leftChild);
        dup->rightChild = duplicate(orig->rightChild);
    }
    return (dup);
}

/**********************************************************/


/**********************************************************/
/******************** Utility Functions *******************/
/**********************************************************/

// structure holding propositional values
typedef struct PropVal {
    char prop;
    int val; // '0' for False and '1' for True
} PV;

// scan every propositional values (one by one) from user as input
PV *scanPropValueInput()
{
    unsigned int noProp, i;
    PV *pvarr;

    printf("Enter Total Number of Propositions: ");
    scanf("%u",&noProp);

    pvarr = (PV *)malloc(noProp * sizeof(PV));

    for (i = 0; i < noProp; i++) {
        printf("Enter Proposition [%u] (Format: Name <SPACE> Value): ", i+1);
        scanf(" %c %d", &pvarr[i].prop, &pvarr[i].val);
    }
    return pvarr;
}

// determines whether p is a proposition including 0 or 1
int isProposition (char p)
{
    return ( ((p >= 'a') && (p <= 'z')) || ((p >= 'A') && (p <= 'Z')) || (p == '0') || (p == '1') );
}

// printing the validity and satisfiability flags
void printResult (int valid, int sat)
{
    printf("\nThe Given Formula is: < ");
    valid ? printf("VALID") : printf("INVALID");
    printf(" + ");
    sat ? printf("SATISFIABLE") : printf("UNSATISFIABLE");
    printf(" >\n\n");
}

//display postfix form of propositional formula (from internally represented string)
void displayPfForm(char *pfForm)
{
    int i;

    printf("Postfix Representation of Formula:");
    for(i = 0; pfForm[i] != '\0'; i++){
        if ( pfForm[i] == '-' )
            printf(" ->");
        else if (pfForm[i] == '~')
            printf(" <->");
        else
            printf(" %c", pfForm[i]);
    }
    printf("\n");
}

// count number of characters in the formula representing only propositions and operators
int noOfIdsInFormula (char *formula)
{
    int i, len = strlen(formula), count = 0;
    for(i = 0; i < len; i++){
        if ( (formula[i] != '(') && (formula[i] != ')') && (formula[i] != ' ') && (formula[i] != '\t') )
            count++;
    }
    return count;
}

// pre-defined priority of in-stack element
int inStackPriority (char operator){
    switch(operator){
        case '!': return 3; break;

        case '&':
        case '|': return 2; break;

        case '~':
        case '-': return 1; break;

        case '(': return 0; break;

        default : return -1; break;
    }
}

// pre-defined priority of in-coming element
int inComingPriority (char operator){
    switch(operator){
        case '!': return 4; break;

        case '&':
        case '|': return 2; break;

        case '~':
        case '-': return 1; break;

        case '(': return 4; break;

        default : return -1; break;
    }
}

// generate postfix formula from the given input formula
char *genPostFixFormula(char *formula)
{
    int noOfIds = noOfIdsInFormula(formula), i, len = strlen(formula), k;
    char *pf = (char *)malloc((noOfIds+1) * sizeof(char));
    char out;
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    initStack(stack); push(stack,'#');

    for (i = 0, k = 0; i < len; i++){
        if ( (formula[i] != ' ') && (formula[i] != '\t') ){
            if ( isProposition(formula[i]) )
                pf[k++] = formula[i];
            else if (formula[i] == ')') {
                while ( (out = pop(stack)) != '(')
                    pf[k++] = out;
            }
            else {
                while ( inStackPriority(out = pop(stack)) >= inComingPriority(formula[i]) )
                    pf[k++] = out;
                push(stack, out);
                push(stack, formula[i]);
            }
        }
    }
    while( (out = pop(stack)) != '#' )
        pf[k++] = out;
    pf[k] = '\0';

    return pf;
}

/**********************************************************/





/**********************************************************/
/****************** YOUR CODE STARTS HERE *****************/
/**********************************************************/

// expression tree formation from postfix formula string
BT *ETF (char *pfForm, int start, int end)
{
    BT *et = NULL;

    if(end==start)//Base Case: One proposition to put as leaf node
    {
        et=createNode(pfForm[end]);
    }
    else if(end>0 && pfForm[end]=='!')//Formation of subtree with '!' as root node and having only a right child
    {
        et=createNode(pfForm[end]);
        et->rightChild=ETF(pfForm,start,end-1);
    }
    else if(end>1 && (pfForm[end]=='&' || pfForm[end]=='|' || pfForm[end]=='-' || pfForm[end]=='~'))//Formation of subtree having both left and right childs
    {
        et=createNode(pfForm[end]);
        int k=end-1,count=0;
        while(count!=-1)
        {
            if(pfForm[k]=='&' || pfForm[k]=='|' || pfForm[k]=='-' || pfForm[k]=='~')//Each binary connective needs two operands
            {
                count++;
            }
            else if(isProposition(pfForm[k]))
            {
                count--;
            }
            k--;
        }
        et->leftChild=ETF(pfForm,start,k);//Recursive call for left operand
        et->rightChild=ETF(pfForm,k+1,end-1);//Recursive call for right operand
    }

    return et;
}

// storing the expresion tree in a string using inorder traversal
void ETP (BT *et)
{
    if(et==NULL)
    {
        return;
    }
    else if(isProposition(et->element) || et->element=='!')//Avoiding usage of unnecessary brackets
    {
        ETP(et->leftChild);
        if(et->element=='-')
        {
            printf("%s","-> ");
        }
        else if(et->element=='~')
        {
            printf("%s","<-> ");
        }
        else
        {
            printf("%c ",et->element);
        }
        ETP(et->rightChild);
    }
    else
    {
        printf("( ");
        ETP(et->leftChild);
        if(et->element=='-')
        {
            printf("%s","-> ");
        }
        else if(et->element=='~')
        {
            printf("%s","<-> ");
        }
        else
        {
            printf("%c ",et->element);
        }
        ETP(et->rightChild);
        printf(")");
    }
}

// evaluate the formula from the expression tree from the proposition values provided in varr[] array
int EVAL (BT *et, PV *pvarr)
{
    int result;

    if(isProposition(et->element))
    {
        int i=0;
        while(1)
        {
            if(pvarr[i].prop==et->element)
            {
                result=pvarr[i].val;
                break;
            }
            i++;
        }
    }
    else if(et->element=='!')//Negation Operation equivalent to inversion
    {
        result=(EVAL(et->rightChild,pvarr)==1)? 0 : 1;
    }
    else if(et->element=='&')//AND Operation equivalent to multiplication
    {
        result=(EVAL(et->leftChild,pvarr))*(EVAL(et->rightChild,pvarr));
    }
    else if(et->element=='|')//OR Operation equivalent to addition
    {
        result=((EVAL(et->leftChild,pvarr))+(EVAL(et->rightChild,pvarr))>0)?1:0;
    }
    else if(et->element=='-')//Implication Operation returns 0 in only one case where antecedent is True but the Consequent is False
    {
        result=(EVAL(et->leftChild,pvarr)==1 && EVAL(et->rightChild,pvarr)==0)? 0 : 1;
    }
    else if(et->element=='~')//Equivalence checks if the operands on either side return the same truth value or not
    {
        result=(EVAL(et->leftChild,pvarr)==EVAL(et->rightChild,pvarr))? 1 : 0;
    }
    return result;
}

// convert expression tree to IFF expression tree
BT *IFF (BT *et)
{
    if(et->element=='!')
    {
        et->rightChild=IFF(et->rightChild);
    }
    else if(et->element=='&' || et->element=='|')
    {
        et->leftChild=IFF(et->leftChild);
        et->rightChild=IFF(et->rightChild);
    }
    else if(et->element=='-')
    {
        BT* newnode=createNode('!');//New node to accommodate the '!' connective
        newnode->rightChild=et->leftChild;//'!' can have only right child
        et->leftChild=IFF(newnode);
        et->element='|';//Implication is transformed to OR
        et->rightChild=IFF(et->rightChild);
    }
    else if(et->element=='~')
    {
        BT* newnode1=createNode('-');//New node to accommodate Implication connective
        newnode1->leftChild=et->leftChild;
        newnode1->rightChild=et->rightChild;
        BT* newnode2=createNode('-');//New node to accommodate Implication connective
        newnode2->leftChild=et->rightChild;
        newnode2->rightChild=et->leftChild;
        et->leftChild=IFF(newnode1);
        et->element='&';//Equivalence demands Implication from both sides
        et->rightChild=IFF(newnode2);
    }

    return et;
}

// convert IFF expression tree to NNF expression tree
BT *NNF (BT *etI)
{
    if(etI->element=='!')
    {
        if(etI->rightChild->element=='!')//Double Negation is resolved
        {
            etI=NNF(etI->rightChild->rightChild);
        }
        else if(etI->rightChild->element=='&')//De Morgan's Theorem ![p&q]=[!p|!q]
        {
            BT* newnode1=createNode('!');//New node to accommodate for '!' statement
            newnode1->rightChild=etI->rightChild->leftChild;
            BT* newnode2=createNode('!');//New node to accommodate for '!' statement
            newnode2->rightChild=etI->rightChild->rightChild;
            etI->leftChild=NNF(newnode1);
            etI->element='|';
            etI->rightChild=NNF(newnode2);
        }
        else if(etI->rightChild->element=='|')//De Morgan's Theorem ![p|q]=[!p&!q]
        {
            BT* newnode1=createNode('!');//New node to accommodate for '!' statement
            newnode1->rightChild=etI->rightChild->leftChild;
            BT* newnode2=createNode('!');//New node to accommodate for '!' statement
            newnode2->rightChild=etI->rightChild->rightChild;
            etI->leftChild=NNF(newnode1);
            etI->element='&';
            etI->rightChild=NNF(newnode2);
        }
    }
    else if(etI->element=='&' || etI->element=='|')
    {
        etI->leftChild=NNF(etI->leftChild);
        etI->rightChild=NNF(etI->rightChild);
    }

    return etI;
}

// convert NNF expression tree to CNF expression tree
BT *CNF (BT *etN)
{
    if(etN->element=='&')
    {
        etN->leftChild=CNF(etN->leftChild);
        etN->rightChild=CNF(etN->rightChild);
    }
    else if(etN->element=='|')//Enforcing the Distribution Law
    {
        if(etN->leftChild->element=='&')//[p&q]|r = [p|r]&[q|r]
        {
            BT* duplr=duplicate(etN->leftChild->rightChild);//Duplicating q i.e. Duplicating the right operand of the AND connective upon which OR is about to be distributed from right
            etN->leftChild->element='|';
            etN->leftChild->rightChild=etN->rightChild;
            BT* newnode=createNode('|');//New node to accommodate OR connective
            newnode->leftChild=duplr;
            newnode->rightChild=etN->rightChild;

            etN->leftChild=CNF(etN->leftChild);
            etN->element='&';
            etN->rightChild=CNF(newnode);
        }
        else if(etN->rightChild->element=='&')//r|[p&q] = [r|p]&[r|q]
        {
            BT* duprl=duplicate(etN->rightChild->leftChild);//Duplicating p i.e. Duplicating the left operand of the AND connective upon which OR is about to be distributed from left
            etN->rightChild->element='|';
            etN->rightChild->leftChild=etN->leftChild;
            BT* newnode=createNode('|');//New node to accommodate OR connective
            newnode->leftChild=etN->leftChild;
            newnode->rightChild=duprl;

            etN->leftChild=CNF(newnode);
            etN->element='&';
            etN->rightChild=CNF(etN->rightChild);
        }
    }

    return etN;
}

// convert NNF expression tree to DNF expression tree
BT *DNF (BT *etN)
{
    if(etN->element=='|')
    {
        etN->leftChild=DNF(etN->leftChild);
        etN->rightChild=DNF(etN->rightChild);
    }
    else if(etN->element=='&')//Enforcing the Distribution Law
    {
        if(etN->leftChild->element=='|')//[p|q]&r = [p&r]|[q&r]
        {
            BT* duplr=duplicate(etN->leftChild->rightChild);//Duplicating q i.e. Duplicating the right operand of the OR connective upon which AND is about to be distributed from right
            etN->leftChild->element='&';
            etN->leftChild->rightChild=etN->rightChild;
            BT* newnode=createNode('&');//New node to accommodate AND connective
            newnode->leftChild=duplr;
            newnode->rightChild=etN->rightChild;

            etN->leftChild=DNF(etN->leftChild);
            etN->element='|';
            etN->rightChild=DNF(newnode);
        }
        else if(etN->rightChild->element=='|')//r&[p|q] = [r&p]|[r&q]
        {
            BT* duprl=duplicate(etN->rightChild->leftChild);//Duplicating p i.e. Duplicating the left operand of the OR connective upon which AND is about to be distributed from left
            etN->rightChild->element='&';
            etN->rightChild->leftChild=etN->leftChild;
            BT* newnode=createNode('&');//New node to accommodate AND connective
            newnode->leftChild=etN->leftChild;
            newnode->rightChild=duprl;

            etN->leftChild=DNF(newnode);
            etN->element='|';
            etN->rightChild=DNF(etN->rightChild);
        }
    }

    return etN;
}

// exhaustive search for checking validity / satisfiability
void CHECK (BT *et)
{
    int valid = 1, sat = 0;
    int n;
    printf("\nEnter Number Of Propositions: ");
    scanf("%d",&n);
    PV parr[n];
    printf("\nEnter Proposition Names (<SPACE> Seperated): ");
    for(int i=0;i<n;i++)
    {
        scanf(" %c",&(parr[i].prop));
        parr[i].val=0;
    }
    int sum=0,result;//sum= To store the sum of results obtained from each row in the truth table
    int total=pow(2,n);//Total number of rows in the truth table
    printf("\nEvaluations of the Formula:");
    for(int i=0;i<total;i++)
    {
        for(int j=0;j<n;j++)
        {
            if(i!=0 && i%((int)(total/pow(2,j+1)))==0)//Check to apply appropriate changes to truth values of the simple propositions
            {
                parr[j].val=(parr[j].val==1)?0:1;
            }
        }
        printf("\n   { ");
        for(int i=0;i<n;i++)
        {
            printf("(%c = %d) ",parr[i].prop,parr[i].val);
        }
        result = EVAL(et, parr);
        printf("}  : %d",result);
        sum=sum+result;
    }
    if(sum==total)//All rows have truth value=1
    {
        valid=1;
        sat=1;
    }
    else if(sum==0)//All rows have truth value=0
    {
        valid=0;
        sat=0;
    }
    else//Neither of the above cases
    {
        valid=0;
        sat=1;
    }
    printResult(valid,sat);
}

/**********************************************************/
/******************* YOUR CODE ENDS HERE ******************/
/**********************************************************/





// main routine
int main ()
{
    char formula[MAX], *pfForm;
    int len, i;

    BT *et, *etI, *etN, *etDup, *etC, *etD;
    int *varr;
    PV *pvarr;
    int result;

    // scan propositional formula from user input
    printf("\nEnter Propositional Logic Formula: ");
    scanf("%[^\n]", formula);

    // internal formula string with operators as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    len = strlen(formula);
    for(i = 0; i < len; i++){
        if(formula[i] == '<'){ // denoting iff operator (<->) using ~
            formula[i] = ' ';
            formula[i+1] = '~';
        }
        if(formula[i] == '>'){ // denoting imply operator (->) using -
            formula[i] = ' ';
        }
    }

    // postfix form generation from represented formula string
    pfForm = genPostFixFormula(formula);

    // display postfix form of the internally represented formula
    displayPfForm(pfForm);

    // internal postfix formula string with operators as, AND (&), OR (|), NOT (!), IMPLY (-) and IFF (~)
    printf("\n++++ PostFix Format of the Propositional Formula ++++\n('-' used for '->' and '~' used for '<->')\n");
    printf("YOUR INPUT STRING: %s\n", pfForm);



    /**********************************************************/
    /********** YOUR CODE ENABLES THE FOLLOWING PARTS *********/
    /**********************************************************/

    printf("\n++++ Expression Tree Generation ++++");
    et = ETF(pfForm, 0, strlen(pfForm)-1);
    printf("\nOriginal Formula (from Expression Tree):");
    ETP(et);
    printf("\n");

    printf("\n++++ Expression Tree Evaluation ++++\n");
    pvarr = scanPropValueInput();
    result = EVAL(et, pvarr);
    printf("\nThe Formula is Evaluated as: ");
    (result) ? printf("True\n") : printf("False\n");

    printf("\n++++ IFF Expression Tree Conversion ++++");
    etI = IFF(et);
    printf("\nFormula in Implication Free Form (IFF from Expression Tree):");
    ETP(etI);
    printf("\n");

    printf("\n++++ NNF Expression Tree Conversion ++++");
    etN = NNF(etI);
    printf("\nFormula in Negation Normal Form (NNF from Expression Tree):");
    ETP(etN);
    printf("\n");

    etDup = duplicate(etN); // keeping a duplicate copy for DNF conversion

    printf("\n++++ CNF Expression Tree Conversion ++++");
    etC = CNF(etN);
    printf("\nFormula in Conjunctive Normal Form (CNF from Expression Tree):");
    ETP(etC);
    printf("\n");

    printf("\n++++ DNF Expression Tree Conversion ++++");
    etD = DNF(etDup);
    printf("\nFormula in Disjunctive Normal Form (DNF from Expression Tree):");
    ETP(etD);
    printf("\n");

    printf("\n++++ Exhaustive Search from Expression Tree for Validity / Satisfiability Checking ++++");
    CHECK (et);

    /**********************************************************/



    return 0;
}
