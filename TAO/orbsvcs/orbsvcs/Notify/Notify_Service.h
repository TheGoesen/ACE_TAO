/* -*- C++ -*- */
/**
 *  @file Notify_Service.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_COSNOTIFY_SERVICE_H
#define TAO_NS_COSNOTIFY_SERVICE_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Service.h"

class TAO_NS_Factory;
class TAO_NS_Builder;
class TAO_NS_Properties;

/**
 * @class TAO_NS_Notify_Service
 *
 * @brief A service object for creating the  Notify Service Factory.
 *
 */
class TAO_Notify_Export TAO_NS_Notify_Service : public TAO_NS_Service
{
public:
  /// Constuctor
  TAO_NS_Notify_Service (void);

  /// Destructor
  virtual ~TAO_NS_Notify_Service ();  

  /// Init
  virtual void init (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);
  
  /// Create the Channel Factory.
  virtual CosNotifyChannelAdmin::EventChannelFactory_ptr create (PortableServer::POA_ptr default_POA ACE_ENV_ARG_DECL);

protected:
  /// Init the data members
  virtual void init_i (CORBA::ORB_ptr orb ACE_ENV_ARG_DECL);

  /// Create the Factory for RT Notify objects.
  virtual void init_factory (ACE_ENV_SINGLE_ARG_DECL);

  /// Creates the Builder for RT Notify objects.
  virtual void init_builder (ACE_ENV_SINGLE_ARG_DECL);

  /// Service component for object factory operations.
  TAO_NS_Factory* factory_;

  /// Service component for building NS participants.
  TAO_NS_Builder* builder_;
};

ACE_FACTORY_DECLARE (TAO_Notify, TAO_NS_Notify_Service)

#if defined (__ACE_INLINE__)
#include "Notify_Service.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_COSNOTIFY_SERVICE_H */

