template <typename T, int64_t block_size> atBlockAllocator<T, block_size>::atBlockAllocator() : m_ppBlocks(nullptr) {}
template <typename T, int64_t block_size> atBlockAllocator<T, block_size>::~atBlockAllocator() {}

template <typename T, int64_t block_size> T * atBlockAllocator<T, block_size>::alloc(const int64_t count)
{
  return NULL;
}

template <typename T, int64_t block_size> T * atBlockAllocator<T, block_size>::realloc(T * pBlock, const int64_t count)
{
  return NULL;
}

template <typename T, int64_t block_size> void atBlockAllocator<T, block_size>::free(T * pBlock)
{
}

template<typename T, int64_t block_size> T * atBlockAllocator<T, block_size>::alloc_block()
{
  atBlock<T> *pBlock = atAllocator::alloc(block_size);
}

template<typename T, int64_t block_size> atBlockAllocator<T, block_size>::alloc_blocks(const int64_t count)
{
  atBlock<T> **ppBlocks = atAllocator::alloc(m_nBlocks + count);
  memcpy(ppBlocks, m_ppBlocks, sizeof(atBlock<T>*) * m_nBlocks);
}

template<typename T, int64_t block_size> atBlockAllocator<T, block_size>::free_block(const int64_t index)
{

  for(int64_t i = index; i < m_nBlocks; ++i)

}
