// -*- C++ -*-

//=============================================================================
/**
 *  @file    Echo_i.h
 *
 *  This class implements the Echo IDL interface.
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ECHO_I_H
#define ECHO_I_H

#include "EchoS.h"

/**
 * @class Echo_i
 *
 * @brief Echo Object Implementation
 *
 * The object implementation  performs the following functions:
 * -- To return the string which needs to be displayed
 * from the server.
 * -- shuts down the server
 */
class Echo_i : public POA_Echo
{
public:
  /// Constructor.
  Echo_i (void) = default;

  /// Destructor.
  virtual ~Echo_i (void) = default;

  /// Return the mesg string back from the server.
  virtual Echo::List *echo_list (const char *mesg);

  /// Return the mesg string back from the server.
  virtual char *echo_string (const char *mesg);

  /// Shutdown the server.
  virtual void shutdown (void);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  void operator= (const Echo_i&);
};

#endif /* ECHO_I_H */
