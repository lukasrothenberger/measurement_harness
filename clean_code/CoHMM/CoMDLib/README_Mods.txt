Modifications from CoMD 1.1
===========================

A much less naive library version of CoMD. This version takes, as
input, a struct containing the information that would otherwise have
been passed as command line parameters. As an output, it generates an
array of structs containing pertinent data (stress tensor) for
multiple time steps.  Stress tensor results casually verified, but
more rigorous checking required.

Theoretically, this approach should work with any version of
CoMD. However, as Dominic pointed out, we are still missing some
useful information that can be generated by CoMD, so the interface
will need to be changed.  Current interface is still slightly
inefficient, but will require minimal changes as more parameters are
needed on in and out.


This version is Thread Safe. The others weren't. Let's not talk about
this too much
