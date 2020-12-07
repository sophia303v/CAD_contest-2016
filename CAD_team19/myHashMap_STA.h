/****************************************************************************
  FileName     [ myHashMap_STA.h ]
  PackageName  [ STA ]
  Synopsis     [ Define my own HashMap  ]
  Author       [ Sophia Hsu ]
  identity     [ Student, NTUEE ,Taiwan]
  date         [ 2016, 06, 02](last update)
****************************************************************************/

#ifndef MY_HASH_MAP_H
#define MY_HASH_MAP_H

#include <vector>
#include <string>

using namespace std;


//-----------------------
// Define HashMap classes
//-----------------------
// To use HashMap ADT, you should define your own HashKey class.
// It should at least overload the "()" and "==" operators.
//
class HashKey
{
public:
	HashKey(string token)  
		 {_token = token;
       _value = 0;
       init(); } //A[0] or a1 ,a2
 

  void init() {
       const char* tokenchar = _token.c_str();
       size_t stringsize = _token.size();
       unsigned long int sum = 0;
       for (int i=0,j=stringsize-1;i<stringsize;i++,j--){
          // int tmp = int(tokenchar[i])<<j;
          // cout<<tokenchar[i]<<" = "<<int(tokenchar[i])<<endl;
           sum = sum*10+int(tokenchar[i]);
       }
       _value = sum;
      // cout<<"key value = "<<_value<<endl;
  }
	unsigned long int operator() () const{
                 return _value;  
        }
	bool operator == (const HashKey& k) const {
                return k()==_value;
        }
        
private:
	string _token;
  unsigned long int _value;
};



template <class HashKey, class HashData>
class HashMap
{
typedef pair<HashKey, HashData> HashNode;

public:
   HashMap() : _numBuckets(0), _buckets(0) {}
   HashMap(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
   ~HashMap() { reset(); }

   // [Optional] TODO: implement the HashMap<HashKey, HashData>::iterator
   // o An iterator should be able to go through all the valid HashNodes
   //   in the HashMap
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   // (_bId, _bnId) range from (0, 0) to (_numBuckets, 0)
   //
   class iterator
   {
      friend class HashMap<HashKey, HashData>;

   public:
      iterator(HashMap<HashKey, HashData>* h = 0, size_t b = 0, size_t bn = 0)
      : _hash(h), _bId(b), _bnId(bn) {}
      iterator(const iterator& i)
      : _hash(i._hash), _bId(i._bId), _bnId(i._bnId) {}
      ~iterator() {} // Should NOT delete HashData

      const HashNode& operator * () const { return (*_hash)[_bId][_bnId]; }
      HashNode& operator * () { return (*_hash)[_bId][_bnId]; }
      iterator& operator ++ () {
         if (_hash == 0) return (*this);
         if (_bId >= _hash->_numBuckets) return (*this);
         if (++_bnId >= (*_hash)[_bId].size()) {
            while ((++_bId < _hash->_numBuckets) && (*_hash)[_bId].empty());
            _bnId = 0;
         }
         return (*this);
      }
      iterator& operator -- () {
         if (_hash == 0) return (*this);
         if (_bnId == 0) {
            if (_bId == 0) return (*this);
            while ((*_hash)[--_bId].empty())
               if (_bId == 0) return (*this);
            _bnId = (*_hash)[_bId].size() - 1;
         }
         else
            --_bnId;
         return (*this);
      }
      iterator operator ++ (int) { iterator li=(*this); ++(*this); return li; }
      iterator operator -- (int) { iterator li=(*this); --(*this); return li; }

      iterator& operator = (const iterator& i) {
         _hash = i._hash; _bId = i._bId; _bnId = i._bnId; return (*this); }

      bool operator != (const iterator& i) const { return !(*this == i); }
      bool operator == (const iterator& i) const {
         return (_hash == i._hash && _bId == i._bId && _bnId == i._bnId); }

   private:
      HashMap<HashKey, HashData>*   _hash;
      size_t                        _bId;
      size_t                        _bnId;
   };

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<HashNode>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<HashNode>& operator [] (size_t i) { return _buckets[i]; }
   const vector<HashNode>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const {
      if (_buckets == 0) return end();
/*
      size_t i = 0;
      while (_buckets[i].empty()) ++i;
      if (i == _numBuckets) return end();
      return iterator(const_cast<HashMap<HashKey, HashData>*>(this), i, 0);
*/
      for (size_t i = 0; i < _numBuckets; ++i)
         if (!_buckets[i].empty())
            return iterator(const_cast<HashMap<HashKey, HashData>*>(this), i, 0);
      return end();
   }
   // Pass the end
   iterator end() const {
      return iterator(const_cast<HashMap<HashKey, HashData>*>(this),
             _numBuckets, 0);
   }
   // return true if no valid data
   bool empty() const {
      for (size_t i = 0; i < _numBuckets; ++i)
         if (_buckets[i].size() != 0) return false;
      return true;
   }
   // number of valid data
   size_t size() const {
      size_t s = 0;
      for (size_t i = 0; i < _numBuckets; ++i) s += _buckets[i].size();
      return s;
   }

   // check if k is in the hash...
   // if yes, update n and return true;
   // else return false;
   bool check(const HashKey& k, HashData& n) const {
      size_t b = bucketNum(k);
      for (size_t i = 0, bn = _buckets[b].size(); i < bn; ++i)
         if (_buckets[b][i].first == k) {
            n = _buckets[b][i].second;
            return true;
         }
      return false;
   }

   // return true if inserted successfully (i.e. k is not in the hash)
   // return false is k is already in the hash ==> 
   bool insert(const HashKey& k, const HashData& d) {
      size_t b = bucketNum(k);
      for (size_t i = 0, bn = _buckets[b].size(); i < bn; ++i)
         if (_buckets[b][i].first == k){
            //cout<<"Error! this key has already in!"<<endl;
            return false;
         }
      _buckets[b].push_back(HashNode(k, d));
      return true;
   }

   // return true if inserted successfully (i.e. k is not in the hash)
   // return false is k is already in the hash ==> still do the insertion
   bool replaceInsert(const HashKey& k, const HashData& d) {
      size_t b = bucketNum(k);
      for (size_t i = 0, bn = _buckets[b].size(); i < bn; ++i)
         if (_buckets[b][i].first == k) {
            _buckets[b][i].second = d;
            return false;
         }
      _buckets[b].push_back(HashNode(k, d));
      return true;
   }

   // Need to be sure that k is not in the hash
   void forceInsert(const HashKey& k, const HashData& d) {
      _buckets[bucketNum(k)].push_back(HashNode(k, d)); }

private:
   // Do not add any extra data member
   size_t                   _numBuckets;
   vector<HashNode>*        _buckets;

   size_t bucketNum(const HashKey& k) const {
        /*  cout<<"k()="<<k()<<endl;
          cout<<"_numBuckets="<<_numBuckets<<endl;
          cout<<"k()%_numBuckets="<< k() % _numBuckets<<endl;*/
      return ( k() % _numBuckets); 
      
      
      
      }

};
#endif
