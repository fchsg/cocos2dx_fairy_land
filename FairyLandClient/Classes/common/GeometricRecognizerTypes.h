#ifndef _GeometricRecognizerTypesIncluded_
#define _GeometricRecognizerTypesIncluded_

/*
* This code taken (and modified) from the javascript version of:
* The $1 Gesture Recognizer
*
*		Jacob O. Wobbrock
* 		The Information School
*		University of Washington
*		Mary Gates Hall, Box 352840
*		Seattle, WA 98195-2840
*		wobbrock@u.washington.edu
*
*		Andrew D. Wilson
*		Microsoft Research
*		One Microsoft Way
*		Redmond, WA 98052
*		awilson@microsoft.com
*
*		Yang Li
*		Department of Computer Science and Engineering
* 		University of Washington
*		The Allen Center, Box 352350
*		Seattle, WA 98195-2840
* 		yangli@cs.washington.edu
*/
#include <math.h>
#include <string>
#include <list>
#include <vector>
#include "cocos2d.h"

using namespace std;

namespace DollarRecognizer
{
	typedef vector<cocos2d::CCPoint>  Path2D;
	typedef Path2D::iterator Path2DIterator;

	class RecognitionResult
	{
	public:
		string name;
		double score;
		RecognitionResult(string name, double score)
		{
			this->name = name;
			this->score = score;
		}
	};
}
#endif