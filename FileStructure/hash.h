#pragma once
// hash.h
#ifndef HASH_H
#define HASH_H

int Hash(const char * key);
// create the primary hash value from a string

int MakeAddress(const char * key, int depth);
// get the primary hash, reverse the bits
// return an address of depth bits

#endif