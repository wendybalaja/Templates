/*
    Starter code for assignment 6, CSC 2/454, Fall 2019
    Provides skeleton of code for a simple hierarchy of set
    abstractions.
    Do not include a `main` function with your submission.
    (Note: Extra Credit - Add documentation that looks great with `doxygen`)
*/

#include <set>
#include <iostream>
#include <string.h>
#include <type_traits>
using std::set;
using std::cout;
using std::string;

/// Naive comparator.
/// Provides a default for any type that has an operator<
/// and an operator==.
///
template<typename T>
class comp {
  public:
    bool precedes(const T& a, const T& b) const {
        return a < b;
    }
    bool equals(const T& a, const T& b) const {
        return a == b;
    }
};

/*!
* Abstract base class from which all sets are derived.
*/
template<typename T, typename C = comp<T>>
class simple_set {
  public:
    /**
     * \brief Destructor
     * 
     * Destructor should be virtual so that we call the right version when saying, e.g.,
     * 
     * \code{.cc}
     * simple_set* S = new derived_set(args);
     * // ...
     * delete S;
     * \endcode
     * 
     */
    virtual ~simple_set<T, C>() { }
    
    /** 
     * \brief Inserts item into set
     * 
     * Returns a ref so you can say, e.g.
     * 
     * \code{.cc}
     * S += a += b += c;
     * \endcode
     * 
     */
    virtual simple_set<T, C>& operator+=(const T item) = 0;
        
    /**
     * \brief Removes item from set, if it was there (otherwise does nothing)
     * 
     * Returns a ref so you can say, e.g.
     * 
     * \code{.cc}
     * S -= a -= b -= c;
     * \endcode
     * 
     */
    virtual simple_set<T, C>& operator-=(const T item) = 0;
    
    /**
     * \brief Indicates whether item is in set
     * 
     * Indicates whether item is in set.
     */
    virtual bool contains(const T& item) const = 0;
};

//---------------------------------------------------------------

/** 
 * Example of a set that implements the simple_set interface.
 * Borrows the balanced tree implementation of the standard template
 * library.  Note that you are NOT to use any standard library
 * collections in your code (though you may use strings and streams).
 * 
 * 'virtual' on simple_set ensures single copy if multiply inherited
 */
template<typename T>
class std_simple_set : public virtual simple_set<T>, protected set<T> {
  public:
    /// will invoke std::~set<T>()
    virtual ~std_simple_set<T>() { }  
    virtual std_simple_set<T>& operator+=(const T item) {
        set<T>::insert(item);
        return *this;
    }
    virtual std_simple_set<T>& operator-=(const T item) {
        (void) set<T>::erase(item);
        return *this;
    }
    virtual bool contains(const T& item) const {
        return (set<T>::find(item) != set<T>::end());
    }
};

//---------------------------------------------------------------

/// Exception
class out_of_bounds { };    

/** 
 * Characteristic array implementation of set.
 * Requires instantiation with guaranteed low and one-more-than-high
 * bounds on elements that can be placed in the set.  Should compile
 * and run correctly for any element class T that can be cast to int.
 * Throws out_of_bounds exception when appropriate.
 */
template<typename T>
class carray_simple_set : public virtual simple_set<T> {
    /// 'virtual' on simple_set ensures single copy if multiply inherited
    /// You'll need some data members here.

   const T L;
   const T H;
   bool* ptr; 
   static const out_of_bounds out_bounds_err;

  public:
    /// fill in these methods:

    /// constructor
    carray_simple_set(const T l, const T h): L(l), H(h) {   
       
      if(l>h) throw out_bounds_err; /*handle exception if left bound is bigger than right bound*/
      ptr = new bool[H-L];/* create an array of booleans */
        // (void) l;  (void) h;
    }

    /// destructor
    virtual ~carray_simple_set() {
        delete ptr;
    }

    virtual carray_simple_set<T>& operator+=(const T item) {
        if( item >= H || item < L) throw out_bounds_err;
        *(ptr+((int)item - (int)L)) = true;
        return *this;
    }
    virtual carray_simple_set<T>& operator-=(const T item) {
        // replace this line:
      if( item >= H || item < L) throw out_bounds_err;
      *(ptr+((int)item - (int)L)) = false;
      // ptr[(int)item - (int)L] = false;
      return *this;
    }
    virtual bool contains(const T& item) const {
       return *(ptr+((int)item - (int)L)) == true;
    }
};

//---------------------------------------------------------------

///
/// Naive hash function object.
/// Provides a default for any type that can be cast to int.
///
template<typename T>
class cast_to_int {
  public:
    int operator()(const T n) {
        return (int) n;
    }
};

/// Exception
class overflow { };         

///
/// Hash table implementation of set.
/// Requires instantiation with guaranteed upper bound on number of elements
/// that may be placed in set.  Throws overflow if bound is exceeded.
/// Can be instantiated without second generic parameter if element type
/// can be cast to int; otherwise requires hash function object.
///
template<typename T, typename F = cast_to_int<T>>
class hashed_simple_set : public virtual simple_set<T> {
    /// 'virtual' on simple_set ensures single copy if multiply inherited
  
   int H; /*hash prime number*/
   int P; /*hash table size*/
   T * ptr; 
   static const overflow overflow_err;
    /// I recommend you pick a hash table size p that is a prime
    /// number >= n, use F(e) % p as your hash function, and rehash
    /// with kF(e) % p after the kth collision.  (But make sure that
    /// F(e) is never 0.)
  public:
    /// constructor
    hashed_simple_set(const int n): H(n), P(bigger_prime(n)) {    
        ptr = new T[P];
    }

    /// destructor
    virtual ~hashed_simple_set() { 
        delete ptr;
    }    

    virtual hashed_simple_set<T, F>& operator+=(const T item) {
        int hash; 
        int item_mod = (F()(item)) % H;

        if(item_mod  == 0){
          hash = 1; /* make sure F(e) is never 0 */
        }else{
          hash = item_mod ;
        }

        if((*(ptr+hash) != item) && (*(ptr+hash) != (T)0)){
          int k = 1;
          while((*(ptr+k*hash) != item) && (*(ptr+k*hash) != (T)0) && k<=H){
            k++;
          }
          if(k>H){
            throw overflow_err;
          }else{
            *(ptr+k*hash) = item; 
          }

        }else{
          *(ptr+hash) = item;
        }

        return *this;
    }
    virtual hashed_simple_set<T, F>& operator-=(const T item) {
        
        int item_mod= (F()(item)) % H;

        for (int i=1; i <= P; i++){
            if((*(ptr+i*item_mod)) == item){
                (*(ptr+i*item_mod)) = (T) 0;
                return *this;
            }
        }
        return *this;
    }

    virtual bool contains(const T& item) const {
       int item_mod = (F()(item)) % H;

       if (item_mod == 0){
        item_mod = 1;
       }
       for (int i=1; i<=P; i++){
        if(*(ptr+i*item_mod) == item) return true;
       }

       return false;
    }
    int bigger_prime(int n){
    	int i = 0;
	//check if there is a prime somewhere within the next 1000 numbers.
	while(i<1000){
	 if(prime(n+i)){
	 	return n+1;
	 }
	 i++;
	}
	return 101;
    }
    bool prime(int n){
    	if(n <= 1){
	    return true;
	}
	for(int i = 2; i < n; i++){
	    if(n % i == 0){
	    return false;
	    }
	}
	return true;

    }


};

//---------------------------------------------------------------

/// Sorted array implementation of set; supports binary search.
/// Requires instantiation with guaranteed upper bound on number of
/// elements that may be placed in set.  Throws overflow if bound is
/// exceeded.
///
/// 'virtual' on simple_set ensures single copy if multiply inherited
template<typename T, typename C = comp<T>>
class bin_search_simple_set : public virtual simple_set<T> {
    /// You'll need some data members here.
     const int MAX;
     int size;
     T *ptr; 
     static const out_of_bounds out_bounds_err;
     static const overflow overflow_err;
     C cmp;


    public:
      /// fill in these methods:

      /// constructor
      bin_search_simple_set(const int max): MAX(max) {   
         
        ptr = new T[max];/* create an array of T's */
        size = 0; // new set has size 0

      }

      /// destructor
      virtual ~bin_search_simple_set() {
          delete ptr;
      }

      virtual bool contains(const T& item) const {
        int index = binary_search_remove(ptr, item, 0, size-1);
        return (index != -1);
      }

      virtual bin_search_simple_set<T, C>& operator+=(const T item) {
        //variable size holds the current size of the array, if it is equal to max at the start
        //of an insert, then we throw overflow.
        if(size == MAX){
            throw overflow_err;
        }

        //we look for a location to insert the element
        int index;
        if(size == 0){
            index = 0;
        }
        else {
            index = binary_search(ptr, item, 0, size-1);

        }

        //printf("index is : %d to insert %d current size is : %d\n",index, (int)item,size);

        //if the index is -1, that means the element already exists in the set
        if(index == -1){
            return *this;
        }
        //now that we have an index, we insert element and push everything over one spot

        //if our index turns out to be the last index, we don't need to push anything over so we just place it there
        if(index == size){
            ptr[index] = item;
        }else{

            int i = size-1;
            while (i >= index) 
            { 
                ptr[i+1] = ptr[i]; 
                i--; 
            } 
            ptr[i+1] = item; 

        }
        size++;
        return *this;



      }

       virtual bin_search_simple_set<T, C>& operator-=(const T item) {


        //we look for a location to remove the element
        int index;

        index = binary_search_remove(ptr, item, 0, size-1);


        //printf("index is : %d to remove %d current size is : %d\n",index, (int)item,size);

        //if the index is -1, that means the element does not exists in the set
        if(index == -1){
            return *this;
        }
        //now that we have an index, we remove element and push everything over one spot

        while (index<= size-1) 
        { 
            ptr[index] = ptr[index+1]; 
            index++; 
        } 
        size--;
        return *this;



      }


      //returns -1 if the element already exists in the array
      int binary_search(T ptr[], T item, int low, int high){
        //printf("high is %d low is %d middle is %d item is %d\n", high,low,((high+low)/2),(int) item);

        if(high <= low){
            if(cmp.precedes(item,ptr[low])){
                return low;
            }else{
                return low+1;
            }
        }
        int mid = (high+low)/2;
        T middle = ptr[mid];
        //returns -1 if the element already exists in the array
        if(cmp.equals(item, middle)){
            return -1;
        }
        if(cmp.precedes(item, middle)){
            return binary_search(ptr, item, low, mid-1);
        }else{
            return binary_search(ptr, item, mid+1, high);
        }


      }

     int binary_search_remove(T ptr[], T item, int low, int high) const{

        // if(high <= low){
        //     return -1; //element not found
        // }
        int mid = (high+low)/2;
        T middle = ptr[mid];
        //returns -1 if the element already exists in the array
        if(cmp.equals(item, middle)){
            return mid;
        }
        if(high <= low){
            return -1; //element not found
        }

        if(cmp.precedes(item, middle)){
            return binary_search_remove(ptr, item, low, mid-1);
        }else{
            return binary_search_remove(ptr, item, mid+1, high);
        }


      }

      void showSet(){
        for(int i = 0; i < size; i++){
            printf("%d\n",(int)ptr[i]);
        }
      }






};

//===============================================================
// RANGE SETS

/// Function object for incrementing.
/// Provides a default for any integral type.
///
template<typename T>
class increment {
    static_assert(std::is_integral<T>::value, "Integral type required.");
  public:
    T operator()(T a) const {
	// cast to integer, increment by 1, recast to (T)
        return (T)(((int) a)+1);
    }
};

/// Range type.  Uses comp<T> by default, but you can provide your
/// own replacement if you want, e.g. for C strings.
///
static const struct { } empty_range;    // exception

/// No destructor needed. You may also find it useful to define the following:
/// 
/// \code{.cc}
/// bool precedes(const range<T, C>& other) const { ...
/// bool overlaps(const range<T, C>& other) const { ...
/// \endcode
template<typename T, typename C = comp<T>>
class range {
    /// represents all elements from L
    T L;        
    /// inclusive?
    bool Linc;  
    /// through H
    T H;        
    /// inclusive?
    bool Hinc;  
    /// can't be static; needs explicit instantiation
    C cmp;      
  public:
    range(const T l, const bool linc, const T h, const bool hinc)
            : L(l), Linc(linc), H(h), Hinc(hinc), cmp() {
        if (cmp.precedes(h, l)
            || (cmp.equals(l, h) && (!Linc || !Hinc))) throw empty_range;
    }
    
    T low() const { return L; }
    bool closed_low() const { return Linc; }
    T high() const { return H; }
    bool closed_high() const {return Hinc; }
    bool contains(const T& item) const {
        return ((cmp.precedes(L, item) || (Linc && cmp.equals(L, item)))
            && (cmp.precedes(item, H) || (Hinc && cmp.equals(item, H))));
    }
    bool precedes(const range<T, C>& other) const {
        if((precedes(H, other.L))||((cmp.equals(H,other.L))&&(!Hinc)&&(!other.Linc))){
           return true; 
        }
        return false;
    }


    bool overlaps(const range<T, C>& other) const {
            if(!((cmp.precedes(other.H,L))||(cmp.precedes(H,other.L)))){
                return true;
            }
            else{
                if(((Hinc)&&(other.Linc)&&(cmp.equals(H, other.L)))||((Linc)&&(other.Hinc)&&(cmp.equals(L, other.H)))){
                    return true;
                }
            }
            return false;
    }

    bool equals(const range<T, C>& other) const {

        if(cmp.equals(H, other.H) && (cmp.equals(L, other.L)) && (Linc == other.Linc) && (Hinc == other.Hinc)){
            return true;
        }
        else{
            return false;
        }
    }

    range merge(const range<T, C>& other) const {

        T newL;
        T newH;
        T newLinc;
        T newHinc;
        if(cmp.precedes(L,other.L)){
            newL = L;
            newLinc = Linc;
        }else if(cmp.equals(L,other.L)){
            newL = L;
            newLinc = other.Linc || Linc;
        }else{
            newL = other.L;
            newLinc = other.Linc;
        }

        if(cmp.precedes(H,other.H)){
            newH = other.H;
            newHinc = other.Hinc;
        }else if(cmp.equals(H,other.H)){
            newH = H;
            newHinc = other.Hinc || Hinc;
        }else{
            newH = H;
            newHinc = Hinc;
        }


    return range<T,C>(newL, newLinc, newH, newHinc);

    }




    
};

// You may find it useful to define derived types with two-argument
// constructors that embody the four possible combinations of open and
// close-ended:
//
// template<typename T, typename C = comp<T>>
// class CCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class COrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OCrange : public range<T, C> { ...
// 
// template<typename T, typename C = comp<T>>
// class OOrange : public range<T, C> { ...

/// This is the abstract class from which all range-supporting sets are derived.
///
template<typename T, typename C = comp<T>>
class range_set : public virtual simple_set<T> {
    // 'virtual' on simple_set ensures single copy if multiply inherited
  public:
    virtual range_set<T, C>& operator+=(const range<T, C> r) = 0;
    virtual range_set<T, C>& operator-=(const range<T, C> r) = 0;
};

//---------------------------------------------------------------

/// As implemented in the standard library, sets contain individual
/// elements, not ranges.  (There are range insert and erase operators, but
/// (a) they use iterators, (b) they take time proportional to the number of
/// elements in the range, and (c) they require, for deletion, that the
/// endpoints of the range actually be in the set.  An std_range_set, as
/// defined here, avoids shortcomings (a) and (c), but not (b).  Your
/// bin_search_range_set should avoid (b), though it will have slow insert
/// and remove operations.  A tree_range_set (search tree -- extra credit)
/// would have amortized log-time insert and remove for individual elements
/// _and_ ranges.
///
template<typename T, typename C = comp<T>, typename I = increment<T>>
class std_range_set : public virtual range_set<T, C>,
                      public std_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    static_assert(std::is_integral<T>::value, "Integral type required.");
    I inc;
  public:
    // The first three methods below tell the compiler to use the
    // versions of the simple_set methods already found in std_simple_set
    // (given true multiple inheritance it can't be sure it should do that
    // unless we tell it).

    virtual std_simple_set<T>& operator+=(const T item) {
        return std_simple_set<T>::operator+=(item);
    }
    virtual std_simple_set<T>& operator-=(const T item) {
        return std_simple_set<T>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return std_simple_set<T>::contains(item);
    }
    virtual range_set<T>& operator+=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low())); r.contains(i); i = inc(i)) {

            *this += i;
        }
        return *this;
    }
    virtual range_set<T>& operator-=(const range<T, C> r) {
        for (T i = (r.closed_low() ? r.low() : inc(r.low())); r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
};

//---------------------------------------------------------------

/// Fill out
template<typename T, typename C = comp<T>, typename I = increment<T>>
class carray_range_set : public virtual range_set<T, C>, public carray_simple_set<T> {
    // 'virtual' on range_set ensures single copy if multiply inherited
    static_assert(std::is_integral<T>::value, "Integral type required.");
    C comp;
    I inc;

    static const out_of_bounds out_bounds_err;

  public:

    /* std_range_set */
    carray_range_set(const T l, const T h) : carray_simple_set<T>(l, h), comp(), inc() {}

    virtual carray_simple_set<T>& operator+=(const T item) {
        return carray_simple_set<T>::operator+=(item);
    }

    virtual carray_simple_set<T>& operator-=(const T item) {
        return carray_simple_set<T>::operator-=(item);
    }

    virtual bool contains(const T& item) const {
        return carray_simple_set<T>::contains(item);
    }

    virtual carray_range_set<T>& operator+=(const range<T, C> r) {
      
        for (T i = (r.closed_low() ? r.low() : inc(r.low()));
                r.contains(i); i = inc(i)) {
            *this += i;
        }
        return *this;
    }

    virtual carray_range_set<T>& operator-=(const range<T, C> r) {

        for (T i = (r.closed_low() ? r.low() : inc(r.low()));r.contains(i); i = inc(i)) {
            *this -= i;

        }
        return *this;
    }
    /**
     * @throws out_of_bounds 
     */
   // virtual carray_range_set<T, C, I>& operator+=(const range<T, C> r) throw(out_of_bounds) = 0;


    /**
     * @throws out_of_bounds 
     */
   // virtual carray_range_set<T, C, I>& operator-=(const range<T, C> r) throw(out_of_bounds) = 0;
};

//---------------------------------------------------------------

/// Fill out

template<typename T, typename F = cast_to_int<T>, typename C = comp<T>, typename I = increment<T>>
class hashed_range_set : public virtual range_set<T, C>, public hashed_simple_set<T, F>{
    C comp;
    I inc;
public:
    hashed_range_set(const int n) : hashed_simple_set<T, F>(n), comp(), inc() {
    }
    virtual hashed_simple_set<T, F>& operator+=(const T item){
        return hashed_simple_set<T,F>::operator+=(item);
    }
    virtual hashed_simple_set<T, F>& operator-=(const T item){
        return hashed_simple_set<T,F>::operator-=(item);
    }
    virtual bool contains(const T& item) const {
        return hashed_simple_set<T,F>::contains(item);
    }
    virtual hashed_range_set<T>& operator+=(const range<T, C> r) {
          for (T i = (r.closed_low() ? r.low() : inc(r.low())); r.contains(i); i = inc(i)) {

            *this += i;
        }
        return *this;
    }
    virtual hashed_range_set<T>& operator-=(const range<T, C> r) {
       for (T i = (r.closed_low() ? r.low() : inc(r.low())); r.contains(i); i = inc(i)) {
            *this -= i;
        }
        return *this;
    }
  };
//---------------------------------------------------------------

/// Fill out
template<typename T, typename C = comp<T>>
class bin_range_set : public virtual range_set<T, C> {
    // 'virtual' on range_set ensures single copy if multiply inherited
     const int MAX;
     int size;
     range<T,C>* ptr;
     static const out_of_bounds out_bounds_err;
     static const overflow overflow_err;
     C cmp;


  public:
    /**
     * @throws overflow
     */
   // virtual bin_range_set<T, C>& operator+=(const range<T, C> r) throw(overflow) = 0;

          /// constructor
      bin_range_set(const int max): MAX(max) {   
         
        ptr = new range<T,C>[max];/* create an array of T's */
        size = 0; // new set has size 0

      }
      //to make sure that the compiler knows exactly what operator to call
      //similar to carray
      virtual bin_search_simple_set<T,C>& operator+=(const T item){
          return bin_search_simple_set<T,C>::operator+=(item);
      }
      virtual bin_search_simple_set<T,C>& operator-=(const T item){
          return bin_search_simple_set<T,C>::operator-=(item);
      }
      virtual bin_range_set<T,C>& operator+=(const range<T, C> item) {
        //variable size holds the current size of the array, if it is equal to max at the start
        //of an insert, then we throw overflow.
        if(size == MAX){
            throw overflow_err;
        }
        // int index;
        // if(size == 0){
        //     index = 0;
        // }
        // else {
        //     index = binary_search(ptr, item, 0, size-1);

        // }

        //test for equality
        //test for overlap, if so merge
        //


      }



};

//===============================================================

/// comparator for C strings
///
class lexico_less {
  public:
    bool precedes(const char *a, const char *b) const {
        return strcmp(a, b) < 0;
    }

    bool equals(const char *a, const char *b) const {
        return strcmp(a, b) == 0;
    }
};


typedef enum{mon, tue, wed, thu, fri} weekday;
