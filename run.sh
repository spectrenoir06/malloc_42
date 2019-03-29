#!/bin/sh 
export DYLD_LIBRARY_PATH=.
<<<<<<< HEAD
export DYLD_INSERT_LIBRARIES="libft_malloc.so" 
export DYLD_FORCE_FLAT_NAMESPACE=1 
$@

=======
export DYLD_INSERT_LIBRARIES="libft_malloc.so"
export DYLD_FORCE_FLAT_NAMESPACE=1
$@
>>>>>>> 1df2bd29d36e63d225487c873076529e6e5ba3ca
