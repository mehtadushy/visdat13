//---------------------------------------------------------------------------
#ifndef SparseLinearAlgebra_templCodeH
#define SparseLinearAlgebra_templCodeH
//---------------------------------------------------------------------------
#include "SparseLinearAlgebra.h"
#include "Dynamiclinearalgebra.hpp"
//---------------------------------------------------------------------------



// ---- SparseVector


template <typename FloatType>
inline SparseVector<FloatType>::SparseVector(const SparseVector<FloatType> &init) {
   this->entries = init.entries;
}

template <typename FloatType>
inline mpcard SparseVector<FloatType>::getNumNonZeroEntries() {
   return entries.size();
}

template <typename FloatType>
inline void SparseVector<FloatType>::removeZeros() {
   vector<Entry> newEntries;
   EIteratorConst pos = entries.begin();
   EIteratorConst end = entries.end();
   while (pos != end) {
      if (pos->value != (FloatType)0) {
         newEntries.push_back(*pos);
      }
      ++pos;
   }
   entries = newEntries;
}

template <typename FloatType>
inline void SparseVector<FloatType>::clear() {
   entries.clear();
}

template <typename FloatType>
inline void SparseVector<FloatType>::refresh() 
{
	for(EIterator	iter = entries.begin();
		iter != entries.end();
		iter++)
	{
		iter->value -= iter->value;
	}
}

template <typename FloatType>
inline const FloatType SparseVector<FloatType>::getEntry(const unsigned index) const {
   EIteratorConst pos = entries.begin();
   EIteratorConst end = entries.end();
   while (pos != end) {
      if (pos->index >= index) {
         if (pos->index == index) {
            return pos->value;
         } else {
            return (FloatType)0;
         }
      }
      ++pos;
   }
   return (FloatType)0;
}

template <typename FloatType>
inline const FloatType SparseVector<FloatType>::operator[](const unsigned index) const {
   return getEntry(index);
}

template <typename FloatType>
inline void SparseVector<FloatType>::setEntry(const unsigned &index, const FloatType value) {
   EIterator pos = entries.begin();
   EIterator end = entries.end();
   while (pos != end) {
      if (pos->index >= index) {
         if (pos->index == index) {
            pos->value = value;
            return;
         } else {
            Entry e; e.index = index; e.value = value;
            entries.insert(pos, e);
            return;
         }
      }
      ++pos;
   }
   Entry e; e.index = index; e.value = value;
   entries.push_back(e);
}

template <typename FloatType>
inline void SparseVector<FloatType>::addEntry(
	const unsigned &index, 
	const FloatType value) 
{
   EIterator pos = entries.begin();
   EIterator end = entries.end();
   while (pos != end) 
	{
      if (pos->index >= index) 
		{
         if (pos->index == index) 
			{
            pos->value += value;
            return;
         } 
			else 
			{
            Entry e; e.index = index; e.value = value;
            entries.insert(pos, e);
            return;
         }
      }
      ++pos;
   }
   Entry e; e.index = index; e.value = value;
   entries.push_back(e);
}

/************************************************************************/
/* Following is added by peter                                                                     */
/************************************************************************/

template <typename FloatType>
inline void SparseVector<FloatType>::addEntries_Binary(
	const DVectorI *m_indices, 
	const DynamicVector<FloatType> *m_values, 
	const card32 numOfEntries)
{
	EIterator		pos		=	entries.begin();
	size_t length; card32 id = 0, offset;
	while (pos != entries.end() && id < numOfEntries) 
	{
		card32 index = (*m_indices)[id];
		if (pos->index >= index) 
		{
			if (pos->index == index) 
			{
				pos->value += (*m_values)[id];
				pos++;
			}
			else 
			{
				Entry e; 
				e.index = index; 
				e.value = (*m_values)[id];
				offset = pos - entries.begin();
				entries.insert(pos, e);
				pos = entries.begin() + offset+1;
			}
			id++;
		}
		else
		{
			EIterator begin = entries.begin();
			length = entries.size()-1;
			if(int((begin+length)->index) < (*m_indices)[id])
				break;
			card32	s_id		= pos - begin + 1;
			card32	e_id		= (card32)length;
			while (s_id < e_id - 1)
			{
				card32 mid = (s_id+e_id)/2;
				pos = begin + mid;
				if(int(pos->index) < (*m_indices)[id])
					s_id = mid;
				else
					e_id = mid;
			}
			pos = begin + s_id;
			if(int(pos->index) < (*m_indices)[id])
				pos++;
		}
	}
	while(id < numOfEntries)
	{
		Entry e; 
		e.index = (*m_indices)[id]; 
		e.value = (*m_values)[id];
		entries.push_back(e);
		id++;
	}
}

template <typename FloatType>
inline void SparseVector<FloatType>::updateEntries_Binary(
												const	DynamicVector<FloatType> *m_values, 
												const	DVectorI		*m_indices,
												const	DVectorI		*m_mul,
												const	card32	numOfElements)
{
	EIterator		begin, pos;
	pos = begin = entries.begin();

	size_t length = entries.size()-1; card32 id = 0, offset;
	for(card32 i = 0; i < numOfElements; i++)
	{
		card32	s_id		= 0;
		if((*m_indices)[i] >= 0)
		{
			if(entries.size() == 0 || (*m_indices)[i]> int32(entries[entries.size()-1].index))
			{
				for(card32 j = 0; j < card32((*m_mul)[i]); j++)
				{
					Entry e;
					e.value = (*m_values)[id];
					e.index = (*m_indices)[i]+j;
					entries.push_back(e);
					id++;
				}
				continue;
			}
			else if((*m_indices)[i] < int32(entries[0].index))
			{
				for(card32 j = 0; j < card32((*m_mul)[i]); j++)
				{
					Entry e;
					e.value = (*m_values)[id];
					e.index = (*m_indices)[i]+j;
					entries.insert(entries.begin()+j, e);
					id++;
				}
				continue;
			}
			else
			{
				binary_Search((*m_indices)[i], 0, (card32)entries.size()-1, offset);
				pos = entries.begin() + offset;
				if(pos->index == (*m_indices)[i])
				{
					for(card32 j = 0; j < card32((*m_mul)[i]); j++)
					{
						pos->value += (*m_values)[id];
						pos++;
						id++;
					}
				}
				else
				{
					for(card32 j = 0; j < card32((*m_mul)[i]); j++)
					{
						Entry e;
						e.value = (*m_values)[id];
						e.index = (*m_indices)[i]+j;
						entries.insert(entries.begin()+offset+j, e);
						id++;
					}
				}
			}
		}
		else
		{
			id += (*m_mul)[i];
		}
	}
}

template <typename FloatType>
inline void SparseVector<FloatType>::get_Value(card32 matrixIndex, FloatType &m_value)
{
	card32	index;
	binary_Search(matrixIndex, 0, (card32)entries.size()-1, index);
	m_value = entries[index].value;
}

template <typename FloatType>
inline void SparseVector<FloatType>::binary_Search(card32 e_index, card32 start, card32 end, card32 &index)
{
	if(entries.size() == 0 || e_index <= entries[start].index)
	{
		index = start;
		return;
	}
	else if(e_index >= entries[end].index)
	{
		index = end;
		return;
	}
	else
	{
		card32		s_id = start, e_id = end;
		EIterator		begin = entries.begin();
		while (s_id < e_id - 1)
		{
			card32	mid = (s_id+e_id)/2;
			EIterator	pos = begin + mid;
			if(int32(pos->index) < e_index)
				s_id = mid;
			else if(pos->index > e_index)
				e_id = mid;
			else
			{
				index = mid;
				return;
			}
		}
		if(entries[s_id].index == e_index)
		{
			index	=	 s_id;
			return;
		}
		else
		{
			index =  e_id;
			return;
		}
	}
}

template<typename FloatType>
inline bool SparseVector<FloatType>::hasEntry(card32 e_index)
{
	if(e_index < entries[0].index || e_index > entries[entries.size()-1].index )
		return false;

	card32	index;
	binary_Search(e_index, 0, (card32)entries.size()-1, index);
	return entries[index].index == e_index;
}

template <typename FloatType>
inline void SparseVector<FloatType>::initialize(const	vector<int>*m_indices)
{
	size_t	m_dimension = m_indices->size();
	entries.resize(m_dimension);
	EIterator		pos = entries.begin(); 
	for(card32 i = 0; i < m_dimension; i++)
	{
		pos->index = (*m_indices)[i];
		pos->value = 0.0;
		pos++;
	}
}

template <typename FloatType>
inline void SparseVector<FloatType>::initialize(const	vector<card32>*	m_indices, const	vector<float32>* m_values)
{
	card32		m_dimension = (card32)m_indices->size();
	entries.resize(m_dimension);
	EIterator		pos = entries.begin(); 
	for(card32 i = 0; i < m_dimension; i++)
	{
		pos->index = (*m_indices)[i];
		pos->value = (*m_values)[i];
		pos++;
	}
}

template <typename FloatType>
inline void SparseVector<FloatType>::addEntries_Merging(
	const DVectorI *m_indices, 
	const DynamicVector<FloatType> *m_values, 
	const card32 numOfEntries)
{
	EIterator		pos	= entries.begin();
	card32		id		=	0,offset;
	while (pos != entries.end() && id < numOfEntries) 
	{
		card32 index = (*m_indices)[id];
		if (pos->index >= index) 
		{
			if (pos->index == index) 
			{
				pos->value += (*m_values)[id];
			}
			else 
			{
				Entry e; 
				e.index = index; 
				e.value = (*m_values)[id];
				offset = pos - entries.begin();
				entries.insert(pos, e);
				pos = entries.begin() + offset+1;
			}
			id++;
		}
		else
		{
			++pos;
		}
	}
	while(id < numOfEntries)
	{
		Entry e; 
		e.index = (*m_indices)[id]; 
		e.value = (*m_values)[id];
		entries.push_back(e);
		id++;
	}
}

/************************************************************************/
/* Above is added by peter                                                                   */
/************************************************************************/

template <typename FloatType>
inline unsigned SparseVector<FloatType>::getNumSparseEntries() const {
   return (unsigned)entries.size();
}

template <typename FloatType>
inline void SparseVector<FloatType>::getSparseEntry(const unsigned entryIndex, unsigned &matrixIndex, FloatType &value) const {
   pAssert(entryIndex < entries.size());
   matrixIndex = entries[entryIndex].index;
   value = entries[entryIndex].value;
}

template <typename FloatType>
inline void SparseVector<FloatType>::pushBackEntry(const unsigned entryIndex, const FloatType value) {
	pAssert(entries.empty() || entries.back().index < entryIndex);
	Entry e;
	e.index = entryIndex;
	e.value = value;
	entries.push_back(e);
}

template <typename FloatType>
inline FloatType SparseVector<FloatType>::getListEntryValue(unsigned listIndex) const {
   pAssert(listIndex < entries.size());
   return entries[listIndex].value;
}

template <typename FloatType>
inline unsigned SparseVector<FloatType>::getListEntryIndex(unsigned listIndex) const {
   pAssert(listIndex < entries.size());
   return entries[listIndex].index;
}

template <typename FloatType>
inline void SparseVector<FloatType>::setListEntryValue(unsigned listIndex, const FloatType &v) {
   pAssert(listIndex < entries.size());
   entries[listIndex].value = v;
}

template <typename FloatType>
inline void SparseVector<FloatType>::setListEntryIndex(unsigned listIndex, unsigned ind) {
   pAssert(listIndex < entries.size());
   entries[listIndex].index = ind;
}

template <typename FloatType>
inline void SparseVector<FloatType>::setNumEntries(unsigned numEntries) {
   entries.resize(numEntries);
}



template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::operator+(const SparseVector<FloatType> &op) const {
   SparseVector<FloatType> result;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         result.entries.push_back(*posThis);
         ++posThis;
      } else if (posThis->index > posOp->index) {
         result.entries.push_back(*posOp);
         ++posOp;
      } else {
         Entry e;
         pAssert(posThis->index == posOp->index);
         e.index = posThis->index;
         e.value = posThis->value + posOp->value;
         result.entries.push_back(e);
         ++posOp;
         ++posThis;
      }
   }
   pAssert(posThis == endThis || posOp == endOp);
   while (posThis != endThis) {
      result.entries.push_back(*posThis);
      ++posThis;
   }
   while (posOp != endOp) {
      result.entries.push_back(*posOp);
      ++posOp;
   }

   return result;
}

template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::operator-(const SparseVector<FloatType> &op) const {
   SparseVector<FloatType> result;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         result.entries.push_back(*posThis);
         ++posThis;
      } else if (posThis->index > posOp->index) {
         Entry e = *posOp; e.value = -e.value;
         result.entries.push_back(e);
         ++posOp;
      } else {
         Entry e;
         pAssert(posThis->index == posOp->index);
         e.index = posThis->index;
         e.value = posThis->value - posOp->value;
         result.entries.push_back(e);
         ++posThis;
         ++posOp;
      }
   }
   pAssert(posThis == endThis || posOp == endOp);
   while (posThis != endThis) {
      result.entries.push_back(*posThis);
      ++posThis;
   }
   while (posOp != endOp) {
      Entry e = *posOp; e.value = -e.value;
      result.entries.push_back(e);
      ++posOp;
   }

   return result;
}

template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::operator-() const {
   SparseVector<FloatType> result; result.entries.reserve(entries.size());
   EIteratorConst pos = entries.begin();
   EIteratorConst end = entries.end();
   while (pos != end) {
      Entry e = *pos;
      e.value = -e.value;
      result.entries.push_back(e);
      ++pos;
   }
   return result;
}

template <typename FloatType>
inline FloatType SparseVector<FloatType>::operator*(const SparseVector<FloatType> &op) const {
   FloatType result = (FloatType)0;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         ++posThis;
      } else if (posThis->index > posOp->index) {
         ++posOp;
      } else {
         result += posThis->value * posOp->value;
         ++posThis;
         ++posOp;
      }
   }
   return result;
}

template <typename FloatType>
inline FloatType SparseVector<FloatType>::operator*(const DynamicVector<FloatType> &op) const {
   #if defined(_DEBUG)
   unsigned dim = op.getDim();
   #endif
   FloatType result = (FloatType)0;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   while (posThis != endThis) {
      pAssert(posThis->index < dim);
      result += op[posThis->index] * posThis->value;
      ++posThis;
   }
   return result;
}

template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::operator*(const FloatType &s) const {
   SparseVector<FloatType> result;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   while (posThis != endThis) {
      Entry e;
      e = *posThis;
      e.value *= s;
      result.entries.push_back(e);
      ++posThis;
   }
   return result;
}

template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::operator/(const FloatType &s) const {
   SparseVector<FloatType> result;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   while (posThis != endThis) {
      Entry e;
      e = *posThis;
      e.value /= s;
      result.entries.push_back(e);
      ++posThis;
   }
   return result;
}

template <typename FloatType>
inline SparseVector<FloatType> SparseVector<FloatType>::componentProduct(const SparseVector<FloatType> &op) const {
   SparseVector<FloatType> result;
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         ++posThis;
      } else if (posThis->index > posOp->index) {
         ++posOp;
      } else {
         Entry e;
         pAssert(posThis->index == posOp->index);
         e.index = posThis->index;
         e.value = posThis->value * posOp->value;
         result.entries.push_back(e);
         ++posOp;
         ++posThis;
      }
   }
   return result;
}

template <typename FloatType>
inline void SparseVector<FloatType>::operator+=(const SparseVector<FloatType> &op) {
   EIterator posThis = entries.begin();
   EIterator endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         ++posThis;
      } else if (posThis->index > posOp->index) {
         Entry e = *posOp;
         posThis = entries.insert(posThis, e);
         ++posThis;
         endThis = entries.end();
         ++posOp;
      } else {
         pAssert(posThis->index == posOp->index);
         posThis->value += posOp->value;
         ++posOp;
         ++posThis;
      }
   }
   pAssert(posThis == endThis || posOp == endOp);
   while (posOp != endOp) {
      entries.push_back(*posOp);
      ++posOp;
   }
}

template <typename FloatType>
inline void SparseVector<FloatType>::operator-=(const SparseVector<FloatType> &op) {
   EIterator posThis = entries.begin();
   EIterator endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         ++posThis;
      } else if (posThis->index > posOp->index) {
         Entry e = *posOp;
         e.value = -e.value;
         posThis = entries.insert(posThis, e);
         ++posThis;
         endThis = entries.end();
         ++posOp;
      } else {
         pAssert(posThis->index == posOp->index);
         posThis->value -= posOp->value;
         ++posOp;
         ++posThis;
      }
   }
   pAssert(posThis == endThis || posOp == endOp);
   while (posOp != endOp) {
      Entry e = *posOp; e.value = -e.value;
      entries.push_back(e);
      ++posOp;
   }
}

template <typename FloatType>
inline void SparseVector<FloatType>::operator*=(const FloatType &s) {
   EIterator posThis = entries.begin();
   EIterator endThis = entries.end();
   while (posThis != endThis) {
      posThis->value *= s;
      ++posThis;
   }
}

template <typename FloatType>
inline void SparseVector<FloatType>::operator/=(const FloatType &s) {
   EIterator posThis = entries.begin();
   EIterator endThis = entries.end();
   while (posThis != endThis) {
      posThis->value /= s;
      ++posThis;
   }
}

template <typename FloatType>
inline void SparseVector<FloatType>::operator=(const SparseVector<FloatType> &op) {
   this->entries = op.entries;
}

template <typename FloatType>
inline bool SparseVector<FloatType>::operator==(const SparseVector<FloatType> &op) const {
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   EIteratorConst posOp = op.entries.begin();
   EIteratorConst endOp = op.entries.end();
   while (posThis != endThis && posOp != endOp) {
      if (posThis->index < posOp->index) {
         if (posThis->value != (FloatType)0) return false;
         ++posThis;
      } else if (posThis->index > posOp->index) {
         if (posOp->value != (FloatType)0) return false;
         ++posOp;
      } else {
         if (posThis->value != posOp->value) return false;
         ++posOp;
         ++posThis;
      }
   }
   while (posThis != endThis) {
      if (posThis->value != (FloatType)0) return false;
      ++posThis;
   }
   while (posOp != endOp) {
      if (posOp->value != (FloatType)0) return false;
      ++posOp;
   }

   return true;
}

template <typename FloatType>
inline bool SparseVector<FloatType>::operator!=(const SparseVector<FloatType> &op) const {
   return !(*this == op);
}

template <typename FloatType>
inline bool SparseVector<FloatType>::operator==(const DynamicVector<FloatType> &op) const {
   EIteratorConst pos = entries.begin();
   EIteratorConst end = entries.end();
   const unsigned n = op.size();
   for (unsigned i=0; i<n; i++) {
      if (pos != end) {
         unsigned index = pos->index;
         pAssert(index >= i);
         if (index > i) {
            if (op[i] != (FloatType)0) return false;
         } else {
            if (op[i] != pos->value) return false;
            ++pos;
         }
      } else {
         if (op[i] != (FloatType)0) return false;
      }
   }
   return true;
}

template <typename FloatType>
inline bool SparseVector<FloatType>::operator!=(const DynamicVector<FloatType> &op) const {
   return !(*this == op);
}

template <typename FloatType>
string SparseVector<FloatType>::toString(bool printZeros, int printZerosLength) const {
   char buffer[1000];
   string result;
   result += '(';
   if (printZeros && printZerosLength > 0) {
      for (int i=0; i<printZerosLength-1; i++) {
         sprintf(buffer, "%g", (*this)[i]);
         result += buffer;
         result += ',';
         result += ' ';
      }
      sprintf(buffer, "%g", (*this)[printZerosLength-1]);
      result += buffer;
   } else {
      if (entries.size() > 0) {
         int n = (int)entries.size();
         for (int i=0; i<n-1; i++) {
            sprintf(buffer, "%d", entries[i].index);
            result += buffer;
            result += ':';
            sprintf(buffer, "%g", entries[i].value);
            result += buffer;
            result += ',';
            result += ' ';
         }
         sprintf(buffer, "%d", entries[n-1].index);
         result += buffer;
         result += ':';
         sprintf(buffer, "%g", entries[n-1].value);
         result += buffer;
      }
   }
   result += ')';
   return result;
}

template <typename FloatType>
void SparseVector<FloatType>::checkConsistency() {
   EIteratorConst posThis = entries.begin();
   EIteratorConst endThis = entries.end();
   int lastIndex = -1;
   while (posThis != endThis) {
      pAssert(lastIndex < (int)posThis->index);
      lastIndex = posThis->index;
      ++posThis;
   }
}




// ---- SparseMatrix





template<typename FloatType>
inline SparseMatrix<FloatType>::SparseMatrix(card32 numRows) {
   theRows.resize(numRows);
}

template<typename FloatType>
inline SparseMatrix<FloatType>::SparseMatrix(const SparseMatrix<FloatType> &init) {
   theRows = init.theRows;
}

template<typename FloatType>
void SparseMatrix<FloatType>::setZero() {
   unsigned n = (unsigned)theRows.size();
   for (unsigned i=0; i<n; i++) {
      theRows[i].clear();
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::clear() {
   setZero();
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::refresh() 
{
	unsigned n = (unsigned)theRows.size();
	for (unsigned i=0; i<n; i++) 
	{
		theRows[i].refresh();
	}
}

template<typename FloatType>
inline card32 SparseMatrix<FloatType>::getRows() const {
   return (card32)theRows.size();
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::setRows(card32 numRows) {
   theRows.resize(numRows);
}

template<typename FloatType>
inline SparseVector<FloatType>& SparseMatrix<FloatType>::operator[](const unsigned &index) {
   return theRows[index];
}

template<typename FloatType>
inline const SparseVector<FloatType>& SparseMatrix<FloatType>::operator[](const unsigned &index) const {
   return theRows[index];
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::removeZeros() {
   const unsigned n = theRows.size();
   for (unsigned i=0; i<n; i++) {
      theRows[i].removeZeros();
   }
}


template<typename FloatType>
inline void SparseMatrix<FloatType>::operator+=(const SparseMatrix<FloatType> &op) {
   const unsigned n = (unsigned)theRows.size();
   pAssert(n == op.theRows.size());
   for (unsigned i=0; i<n; i++) {
      theRows[i] += op.theRows[i];
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::operator-=(const SparseMatrix<FloatType> &op) {
   const unsigned n = (unsigned)theRows.size();
   pAssert(n == op.theRows.size());
   for (unsigned i=0; i<n; i++) {
      theRows[i] -= op.theRows[i];
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::operator*=(const FloatType &op) {
   const unsigned n = (unsigned)theRows.size();
   for (unsigned i=0; i<n; i++) {
      theRows[i] *= op;
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::setToComponentProduct(const SparseMatrix<FloatType> &op) {
   const unsigned n = (unsigned)theRows.size();
   pAssert(n == op.theRows.size());
   for (unsigned i=0; i<n; i++) {
      theRows[i] = theRows[i].componentProduct(op.theRows[i]);
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::operator/=(const FloatType &op) {
   const unsigned n = (unsigned)theRows.size();
   for (unsigned i=0; i<n; i++) {
      theRows[i] /= op;
   }
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::operator=(const SparseMatrix<FloatType> &op) {
   theRows = op.theRows;
}

template<typename FloatType>
inline bool SparseMatrix<FloatType>::operator==(const SparseMatrix<FloatType> &op) const {
   const unsigned n = (unsigned)theRows.size();
   pAssert(n == op.theRows.size());
   for (unsigned i=0; i<n; i++) {
      if (theRows[i] != op.theRows[i]) return false;
   }
   return true;
}

template<typename FloatType>
inline bool SparseMatrix<FloatType>::operator!=(const SparseMatrix<FloatType> &op) const {
   return !(*this == op);
}

template<typename FloatType>
inline DynamicVector<FloatType> SparseMatrix<FloatType>::operator*(const DynamicVector<FloatType> &v) const {
   const unsigned n = (unsigned)theRows.size();
   DynamicVector<FloatType> result(n);
   for (unsigned i=0; i<n; i++) {
      result[i] = theRows[i]*v;
   }
   return result;
}

template<typename FloatType>
inline void SparseMatrix<FloatType>::matVecMult(const DynamicVector<FloatType> &v, DynamicVector<FloatType> &result) const {
   const unsigned n = (unsigned)theRows.size();
   for (unsigned i=0; i<n; i++) {
      result[i] = theRows[i]*v;
   }
}

template<typename FloatType>
inline DynamicVector<FloatType> SparseMatrix<FloatType>::operator*(const SparseVector<FloatType> &v) const {
   const unsigned n = (unsigned)theRows.size();
   DynamicVector<FloatType> result(n);
   for (unsigned i=0; i<n; i++) {
      result[i] = theRows[i]*v;
   }
   return result;
}

template<typename FloatType>
string SparseMatrix<FloatType>::toString(bool printZeros, int printZerosLength) const {
   string result = "[ ";
   const unsigned n = (unsigned)theRows.size();
   for (unsigned i=0; i<n; i++) {
      result += theRows[i].toString(printZeros, printZerosLength);
      if (i != n-1) {
         result += ",\n  ";
      }
   }
   result += " ]";
   return result;
}


template <typename FloatType1, typename FloatType2>
inline void convertSparseVectorType(const SparseVector<FloatType1> &source, SparseVector<FloatType2> &dest) {
   dest.setNumEntries(source.getNumSparseEntries());
   const unsigned n = dest.getNumSparseEntries();
   for (unsigned i=0; i<n; i++) {
      dest.setListEntryIndex(i, source.getListEntryIndex(i));
      dest.setListEntryValue(i, (FloatType2)source.getListEntryValue(i));
   }
}

template <typename FloatType1, typename FloatType2>
inline void convertSparseMatrixType(const SparseMatrix<FloatType1> &source, SparseMatrix<FloatType2> &dest) {
   dest.setRows(source.getRows());
   const unsigned n = dest.getRows();
   for (unsigned i=0; i<n; i++) {
      convertSparseVectorType(source[i], dest[i]);
   }
}






#endif
