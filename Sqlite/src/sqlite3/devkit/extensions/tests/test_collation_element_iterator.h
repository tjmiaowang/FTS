#ifndef TEST_COLLATION_ELEMENT_ITERATOR_H
#define TEST_COLLATION_ELEMENT_ITERATOR_H

void TestCollationElementIterator();
void TestCollationElementIterator_NullArgs();
void TestCollationElementIterator_InvalidUtf8();
void TestCollationElementIterator_TruncatedInput();
void TestCollationElementIterator_CombiningMarkSequence();
void TestCollationElementIterator_LookaheadBufferOverflow();
void TestCollationElementIterator_SimpleElements();

#endif // TEST_COLLATION_ELEMENT_ITERATOR_H
