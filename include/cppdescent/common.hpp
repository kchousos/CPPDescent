#pragma once

typedef void* Pointer;

typedef int (*CompareFunc)(Pointer a, Pointer b);

typedef void (*DestroyFunc)(Pointer value);

typedef unsigned int (*HashFunc)(Pointer);