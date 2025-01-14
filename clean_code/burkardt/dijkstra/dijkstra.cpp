# include <cstdlib>
# include <iostream>
# include <iomanip>
# include <ctime>
# include <omp.h>

using namespace std;

# define NV 6

int main ( int argc, char **argv );
int *dijkstra_distance ( int ohd[NV][NV] );
void find_nearest ( int s, int e, int mind[NV], bool connected[NV], int *d, 
  int *v );
void init ( int ohd[NV][NV] );
void timestamp ( void );
void update_mind ( int s, int e, int mv, bool connected[NV], int ohd[NV][NV], 
  int mind[NV] );

//****************************************************************************80

int main ( int argc, char **argv )

//****************************************************************************80
//
//  Purpose:
//
//    MAIN runs an example of Dijkstra's minimum distance algorithm.
//
//  Discussion:
//
//    Given the distance matrix that defines a graph, we seek a list
//    of the minimum distances between node 0 and all other nodes.
//
//    This program sets up a small example problem and solves it.
//
//    The correct minimum distances are:
//
//      0   35   15   45   49   41
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    This C++ version by John Burkardt.
//
{
  int i;
  int i4_huge = 2147483647;
  int j;
  int *mind;
  int ohd[NV][NV];

  timestamp ( );
  cout << "\n";
  cout << "DIJKSTRA_OPENMP\n";
  cout << "  C++ version\n";
  cout << "  Use Dijkstra's algorithm to determine the minimum\n";
  cout << "  distance from node 0 to each node in a graph,\n";
  cout << "  given the distances between each pair of nodes.\n";
  cout << "\n";
  cout << "  Although a very small example is considered, we\n";
  cout << "  demonstrate the use of OpenMP directives for\n";
  cout << "  parallel execution.\n";
//
//  Initialize the problem data.
//
  init ( ohd );
//
//  Print the distance matrix.
//
  cout << "\n";
  cout << "  Distance matrix:\n";
  cout << "\n";
  for ( i = 0; i < NV; i++ )
  {
    for ( j = 0; j < NV; j++ )
    {
      if ( ohd[i][j] == i4_huge )
      {
        cout << "  Inf";
      }
      else
      {
        cout << "  " << setw(3) <<  ohd[i][j];
      }
    }
    cout << "\n";
  }
//
//  Carry out the algorithm.
//
  mind = dijkstra_distance ( ohd );
//
//  Print the results.
//
  cout << "\n";
  cout << "  Minimum distances from node 0:\n";
  cout << "\n";
  for ( i = 0; i < NV; i++ )
  {
    cout << "  " << setw(2) << i
         << "  " << setw(2) << mind[i] << "\n";
  }
//
//  Free memory.
//
  delete [] mind;
//
//  Terminate.
//
  cout << "\n";
  cout << "DIJKSTRA_OPENMP\n";
  cout << "  Normal end of execution.\n";

  cout << "\n";
  timestamp ( );

  return 0;
}
//****************************************************************************80

int *dijkstra_distance ( int ohd[NV][NV] )

//****************************************************************************80
//
//  Purpose:
//
//    DIJKSTRA_DISTANCE uses Dijkstra's minimum distance algorithm.
//
//  Discussion:
//
//    We essentially build a tree.  We start with only node 0 connected
//    to the tree, and this is indicated by setting CONNECTED[0] = TRUE.
//
//    We initialize MIND[I] to the one step distance from node 0 to node I.
//    
//    Now we search among the unconnected nodes for the node MV whose minimum
//    distance is smallest, and connect it to the tree.  For each remaining
//    unconnected node I, we check to see whether the distance from 0 to MV
//    to I is less than that recorded in MIND[I], and if so, we can reduce
//    the distance.
//
//    After NV-1 steps, we have connected all the nodes to 0, and computed
//    the correct minimum distances.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    This C++ version by John Burkardt.
//
//  Parameters:
//
//    Input, int OHD[NV][NV], the distance of the direct link between
//    nodes I and J.
//
//    Output, int DIJKSTRA_DISTANCE[NV], the minimum distance from node 0 
//    to each node.
//
{
  bool *connected;
  int i;
  int i4_huge = 2147483647;
  int md;
  int *mind;
  int mv;
  int my_first;
  int my_id;
  int my_last;
  int my_md;
  int my_mv;
  int my_step;
  int nth;
//
//  Start out with only node 0 connected to the tree.
//
  connected = new bool[NV];
  connected[0] = true;
  for ( i = 1; i < NV; i++ )
  {
    connected[i] = false;
  }
//
//  Initialize the minimum distance to the one-step distance.
//
  mind = new int[NV];

  for ( i = 0; i < NV; i++ )
  {
    mind[i] = ohd[0][i];
  }
//
//  Begin the parallel region.
//
  //# pragma omp parallel private ( my_first, my_id, my_last, my_md, my_mv, my_step ) \
  shared ( connected, md, mind, mv, nth, ohd )
  {
    my_id = omp_get_thread_num ( );
    nth = omp_get_num_threads ( ); 
    my_first =   (   my_id       * NV ) / nth;
    my_last  =   ( ( my_id + 1 ) * NV ) / nth - 1;
//
//  The SINGLE directive means that the block is to be executed by only
//  one thread, and that thread will be whichever one gets here first.
//
//    # pragma omp single
    {
      cout << "\n";
      cout << "  P" << my_id
           << ": Parallel region begins with " << nth << " threads.\n";
      cout << "\n";
    }
    cout << "  P" << my_id
         << ":  First=" << my_first
         << "  Last=" << my_last << "\n";
//
//  Attach one more node on each iteration.
//
    for ( my_step = 1; my_step < NV; my_step++ )
    {
//
//  Before we compare the results of each thread, set the shared variable 
//  MD to a big value.  Only one thread needs to do this.
//
    //  # pragma omp single 
      {
        md = i4_huge;
        mv = -1; 
      }
//
//  Each thread finds the nearest unconnected node in its part of the graph.
//  Some threads might have no unconnected nodes left.
//
      find_nearest ( my_first, my_last, mind, connected, &my_md, &my_mv );
//
//  In order to determine the minimum of all the MY_MD's, we must insist
//  that only one thread at a time execute this block!
//
  //    # pragma omp critical
      {
        if ( my_md < md )  
        {
          md = my_md;
          mv = my_mv;
        }
      }
//
//  This barrier means that ALL threads have executed the critical
//  block, and therefore MD and MV have the correct value.  Only then
//  can we proceed.
//
//      # pragma omp barrier
//
//  If MV is -1, then NO thread found an unconnected node, so we're done early. 
//  OpenMP does not like to BREAK out of a parallel region, so we'll just have 
//  to let the iteration run to the end, while we avoid doing any more updates.
//
//  Otherwise, we connect the nearest node.
//
   //   # pragma omp single 
      {
        if ( mv != - 1 )
        {
          connected[mv] = true;
          cout << "  P" << my_id
               << ": Connecting node " << mv << "\n";;
        }
      }
//
//  Again, we don't want any thread to proceed until the value of
//  CONNECTED is updated.
//
    //  # pragma omp barrier
//
//  Now each thread should update its portion of the MIND vector,
//  by checking to see whether the trip from 0 to MV plus the step
//  from MV to a node is closer than the current record.
//
      if ( mv != -1 )
      {
        update_mind ( my_first, my_last, mv, connected, ohd, mind );
      }
//
//  Before starting the next step of the iteration, we need all threads 
//  to complete the updating, so we set a BARRIER here.
//
   //   #pragma omp barrier
    }
//
//  Once all the nodes have been connected, we can exit.
//
 //   # pragma omp single
    {
      cout << "\n";
      cout << "  P" << my_id
           << ": Exiting parallel region.\n";
    }
  }

  delete [] connected;

  return mind;
}
//****************************************************************************80

void find_nearest ( int s, int e, int mind[NV], bool connected[NV], int *d, 
  int *v )

//****************************************************************************80
//
//  Purpose:
//
//    FIND_NEAREST finds the nearest unconnected node.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    This C++ version by John Burkardt.
//
//  Parameters:
//
//    Input, int S, E, the first and last nodes that are to be checked.
//
//    Input, int MIND[NV], the currently computed minimum distance from
//    node 0 to each node.
//
//    Input, bool CONNECTED[NV], is true for each connected node, whose 
//    minimum distance to node 0 has been determined.
//
//    Output, int *D, the distance from node 0 to the nearest unconnected 
//    node in the range S to E.
//
//    Output, int *V, the index of the nearest unconnected node in the
//    range S to E.
//
{
  int i;
  int i4_huge = 2147483647;

  *d = i4_huge;
  *v = -1;
  for ( i = s; i <= e; i++ )
  {
    if ( !connected[i] && mind[i] < *d )
    {
      *d = mind[i];
      *v = i;
    }
  }
  return;
}
//****************************************************************************80

void init ( int ohd[NV][NV] )

//****************************************************************************80
//
//  Purpose:
//
//    INIT initializes the problem data.
//
//  Discussion:
//
//    The graph uses 6 nodes, and has the following diagram and
//    distance matrix:
//
//    N0--15--N2-100--N3           0   40   15  Inf  Inf  Inf
//      \      |     /            40    0   20   10   25    6
//       \     |    /             15   20    0  100  Inf  Inf
//        40  20  10             Inf   10  100    0  Inf  Inf
//          \  |  /              Inf   25  Inf  Inf    0    8
//           \ | /               Inf    6  Inf  Inf    8    0
//            N1
//            / \
//           /   \
//          6    25
//         /       \
//        /         \
//      N5----8-----N4
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    30 June 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    This C++ version by John Burkardt.
//
//  Parameters:
//
//    Output, int OHD[NV][NV], the distance of the direct link between
//    nodes I and J.
//
{
  int i;
  int i4_huge = 2147483647;
  int j;

  for ( i = 0; i < NV; i++ )  
  {
    for ( j = 0; j < NV; j++ )
    {
      if ( i == j )
      {
        ohd[i][i] = 0;
      }
      else
      {
        ohd[i][j] = i4_huge;
      }
    }
  }

  ohd[0][1] = ohd[1][0] = 40;
  ohd[0][2] = ohd[2][0] = 15;
  ohd[1][2] = ohd[2][1] = 20;
  ohd[1][3] = ohd[3][1] = 10;
  ohd[1][4] = ohd[4][1] = 25;
  ohd[2][3] = ohd[3][2] = 100;
  ohd[1][5] = ohd[5][1] = 6;
  ohd[4][5] = ohd[5][4] = 8;

  return;
}
//****************************************************************************80

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 July 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

void update_mind ( int s, int e, int mv, bool connected[NV], int ohd[NV][NV], 
  int mind[NV] )

//****************************************************************************80
//
//  Purpose:
//
//    UPDATE_MIND updates the minimum distance vector.
//
//  Discussion:
//
//    We've just determined the minimum distance to node MV.
//
//    For each node I which is not connected yet,
//    check whether the route from node 0 to MV to I is shorter
//    than the currently known minimum distance.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    02 July 2010
//
//  Author:
//
//    Original C version by Norm Matloff, CS Dept, UC Davis.
//    This C++ version by John Burkardt.
//
//  Parameters:
//
//    Input, int S, E, the first and last nodes that are to be checked.
//
//    Input, int MV, the node whose minimum distance to node 0
//    has just been determined.
//
//    Input, bool CONNECTED[NV], is true for each connected node, whose 
//    minimum distance to node 0 has been determined.
//
//    Input, int OHD[NV][NV], the distance of the direct link between
//    nodes I and J.
//
//    Input/output, int MIND[NV], the currently computed minimum distances
//    from node 0 to each node.  On output, the values for nodes S through
//    E have been updated.
//
{
  int i;
  int i4_huge = 2147483647;

  for ( i = s; i <= e; i++ )
  {
    if ( !connected[i] )
    {
      if ( ohd[mv][i] < i4_huge )
      {
        if ( mind[mv] + ohd[mv][i] < mind[i] )  
        {
          mind[i] = mind[mv] + ohd[mv][i];
        }
      }
    }
  }
  return;
}



