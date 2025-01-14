/// \file
/// Computes forces for the 12-6 Lennard Jones (LJ) potential.
///
/// The Lennard-Jones model is not a good representation for the
/// bonding in copper, its use has been limited to constant volume
/// simulations where the embedding energy contribution to the cohesive
/// energy is not included in the two-body potential
///
/// The parameters here are taken from Wolf and Phillpot and fit to the
/// room temperature lattice constant and the bulk melt temperature
/// Ref: D. Wolf and S.Yip eds. Materials Interfaces (Chapman & Hall
///      1992) Page 230.
///
/// Notes on LJ:
///
/// http://en.wikipedia.org/wiki/Lennard_Jones_potential
///
/// The total inter-atomic potential energy in the LJ model is:
///
/// \f[
///   E_{tot} = \sum_{ij} U_{LJ}(r_{ij})
/// \f]
/// \f[
///   U_{LJ}(r_{ij}) = 4 \epsilon
///           \left\{ \left(\frac{\sigma}{r_{ij}}\right)^{12}
///           - \left(\frac{\sigma}{r_{ij}}\right)^6 \right\}
/// \f]
///
/// where \f$\epsilon\f$ and \f$\sigma\f$ are the material parameters in the potential.
///    - \f$\epsilon\f$ = well depth
///    - \f$\sigma\f$   = hard sphere diameter
///
///  To limit the interation range, the LJ potential is typically
///  truncated to zero at some cutoff distance. A common choice for the
///  cutoff distance is 2.5 * \f$\sigma\f$.
///  This implementation can optionally shift the potential slightly
///  upward so the value of the potential is zero at the cuotff
///  distance.  This shift has no effect on the particle dynamics.
///
///
/// The force on atom i is given by
///
/// \f[
///   F_i = -\nabla_i \sum_{jk} U_{LJ}(r_{jk})
/// \f]
///
/// where the subsrcipt i on the gradient operator indicates that the
/// derivatives are taken with respect to the coordinates of atom i.
/// Liberal use of the chain rule leads to the expression
///
/// \f{eqnarray*}{
///   F_i &=& - \sum_j U'_{LJ}(r_{ij})\hat{r}_{ij}\\
///       &=& \sum_j 24 \frac{\epsilon}{r_{ij}} \left\{ 2 \left(\frac{\sigma}{r_{ij}}\right)^{12}
///               - \left(\frac{\sigma}{r_{ij}}\right)^6 \right\} \hat{r}_{ij}
/// \f}
///
/// where \f$\hat{r}_{ij}\f$ is a unit vector in the direction from atom
/// i to atom j.
/// 
///

#include "ljForce.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "constants.h"
#include "mytype.h"
#include "parallel.h"
#include "linkCells.h"
#include "memUtils.h"
#include "CoMDTypes.h"

#define POT_SHIFT 1.0

/// Derived struct for a Lennard Jones potential.
/// Polymorphic with BasePotential.
/// \see BasePotential
typedef struct LjPotentialSt
{
   real_t cutoff;          //!< potential cutoff distance in Angstroms
   real_t mass;            //!< mass of atoms in intenal units
   real_t lat;             //!< lattice spacing (angs) of unit cell
   char latticeType[8];    //!< lattice type, e.g. FCC, BCC, etc.
   char  name[3];	   //!< element name
   int	 atomicNo;	   //!< atomic number  
   int  (*force)(SimFlat* s); //!< function pointer to force routine
   void (*print)(FILE* file, BasePotential* pot);
   void (*destroy)(BasePotential** pot); //!< destruction of the potential
   real_t sigma;
   real_t epsilon;
} LjPotential;

static int ljForce(SimFlat* s);
static void ljPrint(FILE* file, BasePotential* pot);

void ljDestroy(BasePotential** inppot)
{
   if ( ! inppot ) return;
   LjPotential* pot = (LjPotential*)(*inppot);
   if ( ! pot ) return;
   comdFree(pot);
   *inppot = NULL;

   return;
}

/// Initialize an Lennard Jones potential for Copper.
BasePotential* initLjPot(void)
{
   LjPotential *pot = (LjPotential*)comdMalloc(sizeof(LjPotential));
   pot->force = ljForce;
   pot->print = ljPrint;
   pot->destroy = ljDestroy;
   //pot->sigma = 2.315;	                  // Angstrom
   //pot->epsilon = 0.167;                  // eV
   //pot->mass = 63.55 * amuToInternalMass; // Atomic Mass Units (amu)

   //pot->lat = 3.615;                      // Equilibrium lattice const in Angs
   strcpy(pot->latticeType, "FCC");       // lattice type, i.e. FCC, BCC, etc.
   //pot->cutoff = 2.5*pot->sigma;          // Potential cutoff in Angs

   strcpy(pot->name, "Cu");
   pot->atomicNo = 29;

   // LJ potential for testing stress with LAMMPS values
   pot->sigma = 2.27;
   pot->epsilon = 0.583;
   pot->cutoff = 3.0*pot->sigma;
   pot->mass = 1.0 * amuToInternalMass;
   pot->lat = pot->sigma*1.587401052;

   return (BasePotential*) pot;
}

void ljPrint(FILE* file, BasePotential* pot)
{
   LjPotential* ljPot = (LjPotential*) pot;
   fprintf(file, "  Potential type   : Lennard-Jones\n");
   fprintf(file, "  Species name     : %s\n", ljPot->name);
   fprintf(file, "  Atomic number    : %d\n", ljPot->atomicNo);
   fprintf(file, "  Mass             : "FMT1" amu\n", ljPot->mass / amuToInternalMass); // print in amu
   fprintf(file, "  Lattice Type     : %s\n", ljPot->latticeType);
   fprintf(file, "  Lattice spacing  : "FMT1" Angstroms\n", ljPot->lat);
   fprintf(file, "  Cutoff           : "FMT1" Angstroms\n", ljPot->cutoff);
   fprintf(file, "  Epsilon          : "FMT1" eV\n", ljPot->epsilon);
   fprintf(file, "  Sigma            : "FMT1" Angstroms\n", ljPot->sigma);
}

int ljForce(SimFlat* s)
{
   LjPotential* pot = (LjPotential *) s->pot;
   real_t sigma = pot->sigma;
   real_t epsilon = pot->epsilon;
   real_t rCut = pot->cutoff;
   real_t rCut2 = rCut*rCut;

   // zero forces and energy
   real_t ePot = 0.0;
   s->ePotential = 0.0;
   int fSize = s->boxes->nTotalBoxes*MAXATOMS;
   for (int ii=0; ii<fSize; ++ii)
   {
      zeroReal3(s->atoms->f[ii]);
      s->atoms->U[ii] = 0.;
   }

   real_t s6 = sigma*sigma*sigma*sigma*sigma*sigma;

   real_t rCut6 = s6 / (rCut2*rCut2*rCut2);
   real_t eShift = POT_SHIFT * rCut6 * (rCut6 - 1.0);

   //virial stress added here
   real_t localStress[9];
   for (int m=0; m<9; m++) 
   {
      localStress[m] = 0.0;
   }

   int nbrBoxes[27];
   // loop over local boxes
   for (int iBox=0; iBox<s->boxes->nLocalBoxes; iBox++)
   {
      int nIBox = s->boxes->nAtoms[iBox];
      if ( nIBox == 0 ) continue;
      int nNbrBoxes = getNeighborBoxes(s->boxes, iBox, nbrBoxes);
      // loop over neighbors of iBox
      for (int jTmp=0; jTmp<nNbrBoxes; jTmp++)
      {
         int jBox = nbrBoxes[jTmp];

         assert(jBox>=0);

         int nJBox = s->boxes->nAtoms[jBox];
         if ( nJBox == 0 ) continue;

         // calculate energy contribution based on whether
         // the neighbor box is local or remote
         real_t localScale;
         if (jBox < s->boxes->nLocalBoxes)
            localScale = 1.0;
         else
            localScale = 0.5;

         // loop over atoms in iBox
         for (int iOff=iBox*MAXATOMS,ii=0; ii<nIBox; ii++,iOff++)
         {
            int iId = s->atoms->gid[iOff];
            // kinetic energy contribution to the virial stress
            int iSpecies = s->atoms->iSpecies[iOff];
            real_t invMass = 1.0/s->species[iSpecies].mass;
            for (int ii=0; ii<3; ii++) 
            {
               for (int jj=0; jj<3; jj++) 
               {
                  int m = 3*ii + jj;
                  localStress[m] -= s->atoms->p[iOff][ii]*s->atoms->p[iOff][jj]*invMass;
               }
            }
            // loop over atoms in jBox
            for (int jOff=MAXATOMS*jBox,ij=0; ij<nJBox; ij++,jOff++)
            {
               real_t dr[3];
               int jId = s->atoms->gid[jOff];  
               if (jBox < s->boxes->nLocalBoxes && jId <= iId )
                  continue; // don't double count local-local pairs.
               real_t r2 = 0.0;
               for (int m=0; m<3; m++)
               {
                  dr[m] = s->atoms->r[iOff][m]-s->atoms->r[jOff][m];
                  r2+=dr[m]*dr[m];
               }

               if ( r2 > rCut2) continue;

               // Important note:
               // from this point on r actually refers to 1.0/r
               r2 = 1.0/r2;
               real_t r6 = s6 * (r2*r2*r2);
               real_t eLocal = r6 * (r6 - 1.0) - eShift;
               s->atoms->U[iOff] += 0.5*eLocal;
               s->atoms->U[jOff] += 0.5*eLocal;

               ePot += localScale * eLocal;
               // different formulation to avoid sqrt computation
               real_t fr = - 4.0*epsilon*r6*r2*(12.0*r6 - 6.0);
               for (int m=0; m<3; m++)
               {
                  s->atoms->f[iOff][m] -= dr[m]*fr;
                  s->atoms->f[jOff][m] += dr[m]*fr;
               }

               // stress computation
               for (int ii=0; ii<3; ii++) 
               {
                  for (int jj=0; jj<3; jj++) 
                  {
                     int m = 3*ii + jj;
                     localStress[m] -= localScale*fr*dr[ii]*dr[jj];
                  }
               }
            } // loop over atoms in jBox
         } // loop over atoms in iBox
      } // loop over neighbor boxes
   } // loop over local boxes in system

   ePot = ePot*4.0*epsilon;
   s->ePotential = ePot;

   // renormalize stress
   for (int m=0; m<9; m++) 
   {
      // divide by the undeformed volume
      localStress[m] = localStress[m]/s->defInfo->globalVolume;
      // normalize the volume accounting for the strain
      // write to the global array
      s->defInfo->stress[m] = localStress[m]/s->defInfo->strain[9];
   }

   return 0;
}
