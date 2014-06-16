#include <stddef.h>

#include "../utils/utf8_utils.h"
#include "../utils/unichar_utils.h"
#include "utf8_decompose_iterator.h"

static void Utf8DecomposeIterator_advanceToNextInputChar(Utf8DecomposeIterator* self)
{
    if (!DecodeNextUtf8CharToUnichar(&self->inputStr, &self->inputLen, &self->currentChar))
    {
        self->inputStr = NULL;
        self->inputLen = 0;
        self->decompositionLen = 0;
        return;
    }

    if (!UnicharDecompose(self->currentChar, &self->currentCharDecomposition, &self->decompositionLen))
    {
        self->currentCharDecomposition = &self->currentChar;
        self->decompositionLen = 1;
    }
}

void Utf8DecomposeIterator_init(Utf8DecomposeIterator* self, char const* inputStr, int inputLen)
{
    self->inputLen = inputLen;
    self->inputStr = inputStr;
    self->currentCharDecomposition = NULL;
    self->decompositionLen = 0;

    Utf8DecomposeIterator_advanceToNextInputChar(self);
}

nds_bool Utf8DecomposeIterator_getNextCharacter(Utf8DecomposeIterator* self, unichar* character)
{
    if (character == NULL)
        return NDS_FALSE;

    if (self->inputStr != NULL && self->decompositionLen == 0)
        Utf8DecomposeIterator_advanceToNextInputChar(self);

    if (self->inputStr == NULL || self->decompositionLen == 0)
        return NDS_FALSE;

    *character = *self->currentCharDecomposition;
    self->decompositionLen--;
    self->currentCharDecomposition++;

    return NDS_TRUE;
}
