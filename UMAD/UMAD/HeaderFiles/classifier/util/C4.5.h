#ifndef _C45_
#define _C45_

/*************************************************************************/
/*									 */
/*		Definitions used in C4.5				 */
/*              ------------------------				 */
/*									 */
/*************************************************************************/
#define	 Eof			EOF             /*char read on end of file*/
#define	 Nil			0               /*null pointer*/
#define	 false			0 
#define	 true			1 
#define	 None			-1
#define	 Epsilon        1E-3

#define	 Random			((rand()&2147483647) / 2147483648.0)

#define	 Max(a,b)               ((a)>(b) ? a : b) 
#define	 Min(a,b)               ((a)<(b) ? a : b) 
#define	 Round(x)		((int) (x+0.5))
#define	 Log2			0.69314718055994530942
#define	 Log(x)			((x) <= 0 ? 0.0 : log((float)x) / Log2)

#define	 Bit(b)			(1 << (b))
#define	 In(b,s)		((s[(b) >> 3]) & Bit((b) & 07))
#define	 ClearBits(n,s)		memset(s,0,n)
#define	 CopyBits(n,f,t)	memcpy(t,f,n)
#define	 SetBit(b,s)		(s[(b) >> 3] |= Bit((b) & 07))

#define	 ForEach(v,f,l)		for(v=f ; v<=l ; ++v) 

#define	 Verbosity(d)		if(VERBOSITY >= d)

#define  Space(s)	(s == ' ' || s == '\n' || s == '\t' || s == '\r')
#define  SkipComment	while ( ( c = getc(f) ) != '\n' )
#define  Inc 2048
#define	 Check(v,l,h)\
	     if ( v<l||v>h ) {printf("\t** illegal value **\n"); exit(1);}

#define	 Tab		"|   "
#define	 TabSize		4
#define	 Width		80	/* approx max width of printed trees */
#define	LocalVerbosity(x)	if (Sh >= 0 && VERBOSITY >= x)
#define	Intab(x)		Indent(x, "| ")
#define	Below(v,t)	(v <= t + 1E-6)

/*************************************************************************/
/*									 */
/*		Type definitions for C4.5				 */
/*              -------------------------				 */
/*									 */
/*************************************************************************/
typedef  char	Boolean, *String, *Set;

typedef  int	ItemNo;		/* data item number */
typedef  float	ItemCount;	/* count of (partial) items */

typedef  short	ClassNo,	/* class number, 0..MaxClass */
				DiscrValue;	/* discrete attribute value (0 = ?) */
typedef  short	Attribute;	/* attribute number, 0..MaxAtt */

typedef  union  _attribute_value
	 {
		short	_discr_val;
	    float	_cont_val;
	 }
	 	AttributeValue, *Descript;

#define  CVal(Case,Attribute)   Case[Attribute]._cont_val
#define  DVal(Case,Attribute)   Case[Attribute]._discr_val
#define  Class(Case)			Case[MaxAtt+1]._discr_val

#define  Unknown  -999		/* unknown value for continuous attribute ：连续属性缺省值默认为-999*/


#define  BrDiscr		1	/* node types:	branch */
#define  ThreshContin	2	/*		threshold cut */
#define  BrSubset		3	/*		subset test */

typedef struct _tree_record *Tree,TreeRec;
struct _tree_record
	 {
	    short		NodeType;	/* 0=leaf 1=branch 2=cut 3=subset */
	    short		Leaf;		/* most frequent class at this node */
	    float		Items,		/* no of items at this node */
			        *ClassDist,	/* class distribution of items */
	    		    Errors;		/* no of errors at this node */
	    short		Tested; 	/* attribute referenced in test */
	    short		Forks;		/* number of branches at this node */
	    float		Cut,		/* threshold for continuous attribute */
		  			Lower,		/* lower limit of soft threshold */
		  			Upper;		/* upper limit ditto */
	    Set			*Subset;	/* subsets of discrete values  */
	    Tree		*Branch;	/* Branch[x] = (sub)tree for outcome x */
	 };

#define  IGNORE		1	/* special attribute status: do not use */
#define  DISCRETE	2	/* ditto: collect values as data read */

typedef short	RuleNo;			/* rule number */


typedef struct TestRec
       {
	   short		NodeType;	/* test type (see tree nodes) */
	   Attribute	Tested;		/* attribute tested */
	   short		Forks;		/* possible branches */
	   float		Cut;		/* threshold (if relevant) */
	   Set			*Subset;	/* subset (if relevant) */
       }*TestTree;


typedef struct CondRec
       {
	   TestTree		CondTest;	/* test part of condition */
	   short	TestValue;	/* specified outcome of test */
       }*Condition;


typedef struct ProdRuleRec
       {
	   short		Size;		/* number of conditions */
	   Condition	*Lhs;		/* conditions themselves */
	   ClassNo		Rhs;		/* class given by rule */
	   float		Error,		/* estimated error rate */
					Bits;		/* bits to encode rule */
	   ItemNo		Used,		/* times rule used */
					Incorrect;	/* times rule incorrect */
       }PR;


typedef struct RuleSetRec
       {
	   PR		*SRule;		/* rules */
	   RuleNo	SNRules,	/* number of rules */
				*SRuleIndex;	/* ranking of rules */
	   ClassNo	SDefaultClass;	/* default class for this ruleset */
		}RuleSet;


#endif
