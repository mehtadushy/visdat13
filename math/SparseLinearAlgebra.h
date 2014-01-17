//---------------------------------------------------------------------------
#ifndef SparseLinearAlgebraH
#define SparseLinearAlgebraH
//---------------------------------------------------------------------------
#include "DynamicLinearAlgebra.h"
#include "PTypes.h"
//---------------------------------------------------------------------------



/// a sparse vector represented by list of (index, value) pairs
/// efficient if a small number (O(1)) entries are non-zero
template <typename FloatType>
class SparseVector {
 private:
   struct Entry {
      unsigned index;
      FloatType value;
   };
   /// vector with entries, sorted by index (ascending)
public:
   vector<Entry> entries;
   typedef typename std::vector<Entry> EVec;
   typedef typename vector<Entry>::iterator EIterator;
   typedef typename vector<Entry>::const_iterator EIteratorConst;

 public:
   //  ---- constructors, copy constructor, adm stuff
   inline SparseVector<FloatType>() {}
   inline SparseVector<FloatType>(const SparseVector<FloatType> &init);
   inline mpcard getNumNonZeroEntries();
   /// remove all entries that contain only zeros
   inline void removeZeros();
   /// set all entries to zero (i.e. remove all entries from list)
   inline void clear();
	/// set the values of all entries to zero
	inline void refresh();

   // ---- element access
   inline const FloatType getEntry(const unsigned index) const;
   inline const FloatType operator[](const unsigned index) const;
   inline void setEntry(const unsigned &index, const FloatType value);
   inline void addEntry(const unsigned &index, const FloatType value);
	
	/************************************************************************/
	/* Following is added by peter                                                                     */
	/************************************************************************/
	/// This method is designed for adding group entries
   /************************************************************************/
   /* Combine the two vector using binary search                                                                     */
   /************************************************************************/
	inline	void	addEntries_Binary(
		const DVectorI *m_indices, 
		const DynamicVector<FloatType> *m_values, 
		const card32 numOfEntries);

	inline	void	updateEntries_Binary(
		const	DynamicVector<FloatType> *m_values, 
		const	DVectorI		*m_indices,
		const	DVectorI		*m_mul,
		const	card32	numOfElements);

	inline	void	get_Value(card32 matrixIndex, FloatType &m_value);

	inline	void	binary_Search(card32 e_index, card32 start, card32 end, card32 &index);

	inline	bool	hasEntry(card32	index);

	inline	void	initialize(const	vector<int>*m_indices);

	inline	void	initialize(const	vector<card32>* m_indices, const	vector<float32>*	m_values);

	/************************************************************************/
	/* Combine the two vectors using merging sort                                                                     */
	/************************************************************************/
	inline	void	addEntries_Merging(
		const DVectorI *m_indices, 
		const DynamicVector<FloatType> *m_values, 
		const card32 numOfEntries);
	
	/************************************************************************/
	/* Above is added by peter                                                                                             */
	/************************************************************************/
	/// number of stored (potentially non-zero) entries
   inline unsigned getNumSparseEntries() const;
   /// get entry from list of stored entries
   inline void getSparseEntry(const unsigned entryIndex, unsigned &matrixIndex, FloatType &value) const;
	/// add entry at the end of the vector; entryIndex must be larger than all indices already present
   inline void pushBackEntry(const unsigned entryIndex, const FloatType value);
   /// element list access
   inline FloatType getListEntryValue(unsigned listIndex) const;
   inline unsigned getListEntryIndex(unsigned listIndex) const;
   inline void setListEntryValue(unsigned listIndex, const FloatType &v);
   inline void setListEntryIndex(unsigned listIndex, unsigned ind);
   inline void setNumEntries(unsigned numEntries);

   //  ----  operators
   inline SparseVector<FloatType> operator+(const SparseVector<FloatType> &op) const;
   inline SparseVector<FloatType> operator-(const SparseVector<FloatType> &op) const;
   inline SparseVector<FloatType> operator-() const;
   inline FloatType operator*(const SparseVector<FloatType> &op) const;
   inline FloatType operator*(const DynamicVector<FloatType> &op) const;
   inline SparseVector<FloatType> operator*(const FloatType &s) const;
   inline SparseVector<FloatType> operator/(const FloatType &s) const;
   inline SparseVector<FloatType> componentProduct(const SparseVector<FloatType> &op) const;
   inline void operator+=(const SparseVector<FloatType> &op);
   inline void operator-=(const SparseVector<FloatType> &op);
   inline void operator*=(const FloatType &s);
   inline void operator/=(const FloatType &s);

   inline void operator=(const SparseVector<FloatType> &op);
   inline bool operator==(const SparseVector<FloatType> &op) const;
   inline bool operator!=(const SparseVector<FloatType> &op) const;
   inline bool operator==(const DynamicVector<FloatType> &op) const;
   inline bool operator!=(const DynamicVector<FloatType> &op) const;

   //  ---- misc.
   string toString(bool printZeros = false, int printZerosLength = 0) const;
   /// assert sorted indices
   void checkConsistency();
};


/// Sparse matrix. stores rows as sparse vectors.
/// Attention: the matrix elements are stored in row/column order
/// (in contrast to DynamicMatrix, StaticMatrix).
template<typename FloatType>
class SparseMatrix {
 private:
   vector< SparseVector<FloatType> > theRows;
 public:
   // ---- constructors, copy constructor
   inline SparseMatrix(card32 numRows = 0);
   inline SparseMatrix(const SparseMatrix<FloatType> &init);

   /// set all entries to zero
   void setZero();
   /// alias for set zero
   inline void	clear();
	/// set the values of all entries to zero
	inline void	refresh();

   inline card32 getRows() const;
   inline void setRows(card32 numRows);

   /// retrieve ROW vector (not column!)
   inline SparseVector<FloatType>& operator[](const unsigned &index);
   /// retrieve ROW vector (not column!)
   inline const SparseVector<FloatType>& operator[](const unsigned &index) const;

   /// remove all zero entries in all vectors
   inline void removeZeros();

   //  ---- operators +=, -=, *=, /=
   inline void operator+=(const SparseMatrix<FloatType> &op);
   inline void operator-=(const SparseMatrix<FloatType> &op);
   inline void operator*=(const FloatType &op);
   inline void setToComponentProduct(const SparseMatrix<FloatType> &op);
   inline void operator/=(const FloatType &op);

   //  ---- operators =, ==, !=
   inline void operator=(const SparseMatrix<FloatType> &op);
   inline bool operator==(const SparseMatrix<FloatType> &op) const;
   inline bool operator!=(const SparseMatrix<FloatType> &op) const;

   // ---- multiplying with dynamic vectors. input can be of arbitrary length. output is of length "getRows()"
   inline DynamicVector<FloatType> operator*(const DynamicVector<FloatType> &v) const;
   /// same as operator*(DynamicVector), but faster. result must be of length "getRows()"
   inline void matVecMult(const DynamicVector<FloatType> &v, DynamicVector<FloatType> &result) const;

   // ---- multiplying with sparse vectors
   inline DynamicVector<FloatType> operator*(const SparseVector<FloatType> &v) const;

   // ----  misc.
   string toString(bool printZeros = false, int printZerosLength = 0) const;
};


// instances
typedef SparseVector<int32> SparseVectorI;
typedef SparseVector<float32> SparseVectorF;
typedef SparseVector<float64> SparseVectorD;

typedef SparseMatrix<int32> SparseMatrixI;
typedef SparseMatrix<float32> SparseMatrixF;
typedef SparseMatrix<float64> SparseMatrixD;


/// perform various tests of sparse linear algebra operations to test implementation
void sparse_linear_algebra_unitTest();


template <typename FloatType1, typename FloatType2>
inline void convertSparseVectorType(const SparseVector<FloatType1> &source, SparseVector<FloatType2> &dest);

template <typename FloatType1, typename FloatType2>
inline void convertSparseMatrixType(const SparseMatrix<FloatType1> &source, SparseMatrix<FloatType2> &dest);



#endif
