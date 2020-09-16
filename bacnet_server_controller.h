/*******************************************************************************
 * Copyright (c) 2020 Alexander Tepaev github.com/alexandertepaev
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *   Alexander Tepaev
 *******************************************************************************/

#ifndef _BACNET_SERVER_CONTROLLER_H_
#define _BACNET_SERVER_CONTROLLER_H_

#include <fortelist.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <arpa/inet.h>

#include <core/io/device/io_controller.h>
#include "bacnet.h"

class CBacnetObject;
class CBacnetCOVReportingObject;

/*! @brief BACnet Server Controller

 * This class represents BACnet Server Controller, whose main objective is to run a separate execution thread.
 * Server controller is responsible for receiving and processing BACnet service requests sent over the underlying 
 * BACnet/IP network.
 * 
 * Features:
 *    + Created and maintained by the server configuration fb upon the initalization.
 *    + Creates and maintains table of BACnet objects (mObjectTable)
 *    + Runs an infinite loop in another thread, while the thread is alive (see run method).
 *    + Supports Who-Is, SubscribeCOV (unconfirmed), ReadProperty, WriteProperty service requests
 *    + Issues SimpleAck, ComplexAck, I-Am, UnconfirmedCOVNotification responses
 */
class CBacnetServerController: protected CThread, protected CExternalEventHandler {
  friend class CBacnetServerConfigFB;
public:
  CBacnetServerController(CDeviceExecution& paDeviceExecution);
  ~CBacnetServerController();

  /*! @brief Adds a new object into the mObjectTable object table
   *
   *  This method updated server controller's object table with the object provided as a parameter. Called by the object config fbs upon initialization.
   * @param paObject Pointer to object instance
   * @return 'true' in case of success, 'false' otherwise
   */
  bool addObjectTableEntry(CBacnetObject *paObject);
  
  /*! @brief Creates a new entry in the COV reporters mCOVReporters list.
   *
   * This method is called by the object config fbs that were configured to issue cov reports
   * upon the initialization. Creates a new entry in the mCOVReporters list with an empty list
   * list of subscription data. See SSubscriptionData and SCOVReporter structs and mCOVReporters list.
   * 
   * @param paObject Pointer to the COV reporting object
   */
  void addCOVReportersEntry(CBacnetCOVReportingObject *paObject);

protected:
    
private:


  TForteUInt8 mSendBuffer[MAX_MPDU]; //!< Buffer for outgoing packets
  TForteUInt8 mReceiveBuffer[MAX_MPDU]; //!< Buffer for incoming packets

  //! Server controller's configuration struct, initialized in init() method
  struct ServerConfig {
    TForteUInt16 nPort;
    struct in_addr stLocalAddr;
    struct in_addr stBroadcastAddr;
  };
  ServerConfig m_stServerConfig = { };

  typedef int TSocketDescriptor; 
  TSocketDescriptor mCommunicationSocket; //!< Socket descriptor
  
  //! Name of the BACnet/IP network interface (e.g, see ifconfig command)
  constexpr static const char * const scm_sNetwrokIfaceName = "wlan0";
  //!< Socket initialization failed message
  constexpr static const char* const scmSocketInitFailed = "Communication socket initialization failed";
  //! constants for timeout/deadlines calculations
  constexpr static TForteUInt16 scm_nMicrosInMillis = 1000;
  //! Timeout, after which select system call unblocks execution (see receivePacket)
  constexpr static TForteUInt8 scm_nReceivePacketSelectTimeoutMillis = 100;


  /*! Object table containing pointers to CBacnetObject instances
   *  addObjectTableEntry -- adds new entry into the table
   *  getBacnetObject --  returns pointer to object instace base on the provided object type and id
   *  mObjectTable->begin() always points to device object instance
   */
  typedef CSinglyLinkedList<CBacnetObject *> TObjectTable;
  TObjectTable *mObjectTable;

  //! Subscription data struct
  struct SSubscriptionData {
    in_addr stAddr; // Address of the subscriber
    TForteUInt16 nPort; // Port of the subscriber
    BACNET_SUBSCRIBE_COV_DATA stCOVData; // COV Data struct (see libbacnet's cov.h)
    SSubscriptionData(in_addr paAddr, TForteUInt16 paPort, BACNET_SUBSCRIBE_COV_DATA paCovData) : stAddr(paAddr), nPort(paPort), stCOVData(paCovData) {
    }
  };
  typedef CSinglyLinkedList<struct SSubscriptionData *> TSubscriptions;
  

  //! Struct for holing COV reporting object and its subscriptions 
  struct SCOVReporter {
    CBacnetCOVReportingObject *pObj; // Pointer to object instance
    TSubscriptions *pSubs; // Pointer to the list of subscriptions, used when issuing notifications
    SCOVReporter(CBacnetCOVReportingObject *paObj, TSubscriptions *paSubs) : pObj(paObj), pSubs(paSubs){}
  };
  typedef CSinglyLinkedList<SCOVReporter *> TCOVReporters;
  TCOVReporters *mCOVReporters; //!< List of objects that were configured to issue COV notifications upon change of their (only present value atm) property

  /*! @brief Initializes server controller
   *
   * In this method, server controller's internals are initialized: object table and cov 
   * reporters lists are created, m_stServerConfig is set (port + local and broadcast addresses), 
   * and Bacnet/IP socket is created. 
   * 
   * @param paPort Port number of the BACnet/IP socket
   * @return 'true' if initialization succeeded, 'false' otherwise 
   */
  const char* init(TForteUInt16 paPort);

  /*! @brief Performs deinitialization of the server controller controller 
   *
   * In this method the initialization of the server controller is performed by closing the communication socket.
   */
  void deinit();

  /*! @brief Queries network addresses of the local network interface
   *
   *  Gets the local address and the subnet mask using the ioctl() system call, calculates 
   *  the broadcast address and sets stLocalAddr and stBroadcastAddr fields of the m_stServerConfig 
   *  struct.
   *  Attention - name of the used network interface should be provided via scm_sNetwrokIfaceName.
   * 
   * // FIXME: error checking
   */
  void setNetworkAddresses();

  /*! @brief Initializes communication socket
   *
   * Creates, configures and binds a UDP socket used for BACnet/IP communication.
   * Initialized socket can be accessed via mCommunicationSocket member variable.
   * 
   *  @return 'true' in case of a success, 'false' in case of a failure
   */
  bool initCommunicationSocket();

  /*! @brief Signals the end of the daisy chained initializaiton to server controller
   *
   * This method is called by the CBacnetServerConfigFB after the end of the initialization of the
   * configuration daisy chain.
   * Starts server controller's execution thread.
   * 
   */
  void initDone();

  /*! @brief Represents the main loop of the server controller's thread
   *
   * This method represents the loop which is executed by the server controller thread
   * Inside the loop, controllers tries to receive a packet by calling receivePacket() 
   * with a 100ms timeout.
   * If a packet is received, server controllers handles it by calling handleReceivedPacket().
   * In addition to that, every loop run server controller processes pending COV notifications
   * by calling preocessPendingCOVReports() method.
   */
  void run();


  /*! @brief Perform address conversion to the format required by libbacnet
   *
   * @param paDeviceAddr Address struct that has to be converted
   * @param paPort Port number
   * @param paBroadcastAddr Boolean parameter, telling if the address is a broadcast address (true if so, false otherwise)
   * @return BACNET_ADDRESS struct provided by libbacnet
   */
  BACNET_ADDRESS ipToBacnetAddress(const struct in_addr &paAddress, const TForteUInt16 &paPort, const bool &paIsBroadcastAddr);

  /*! @brief Send the data contained in the mSendBuffer over the network
   * 
   * This method is used for sending out a service request that is encoded in the mSendBuffer member 
   * over the mCommunicationSocket socket. Uses sendto() system call. 
   * 
   * @param len Length of the to-be-sent packet
   * @param destinationAddr Struct with destination address
   * @param destinationPort Integer indicating destination port
   * @return Number of sent bytes
   */
  int sendPacket(const TForteUInt16 &len, const struct in_addr &destinationAddr, const TForteUInt16 &destinationPort);

  /*! @brief Tries to receive a packet in a non-blocking fashion
   *
   * Performs a non-blocking receive over the mCommunicationSocket socket using select() and recvfrom() system calls.
   * Saves the received data into mReceiveBuffer buffer and stores the source address into the provided src address struct
   * 
   * @param timeout Amount of milliseconds, during which select() checks if the socket fd is ready to receive a packet
   * @param src Address sctruct in which the source address of the received packet is saved
   * @return Number received bytes, or 0 if own packet is received (broadcast case), or -1 if nothing 
   */
  int receivePacket(const TForteUInt16 &timeout, sockaddr_in *sourceAddress);

  /*! @brief Handles received BACnet packet
   *
   * In this method a received BACnet packet is handled.
   * Fisrt of all, its NPDU and APDU portions get decoded by calling the decodeNPDU() and
   * the decodeAPDU() methods. 
   * Afer decoding the request, it calls a specific handle method based on the type 
   * of the request. (Atm only ReadProperty, WriteProperty, Who-Is, SubscribeCOV requests
   * can be processes)
   * 
   * @param paSourceAddress Reference to struct containing source address of the request
   * 
   */
  void handleReceivedPacket(sockaddr_in &paSourceAddress);

  /*! @brief Decodes NPDU portion of a packet and returns APDU offset and APDU length 
   *  
   * In this method NPDU portion of a received BACnet request is decoded and the provided
   * references to APDU offset and APDU length variables are set. Called in combination with
   * decodeAPDU() method.
   * 
   * @param paAPDUOffset Reference to the variable indicating the offset to the APDU portion
   * @param paAPDULen Reference to the variable indication length of the APDU portion
   * @return 'true' in case of a success, 'false' otherwise
   *  
   */
  bool decodeNPDU(TForteUInt16 &paAPDUOffset, TForteUInt16 &paAPDULen);


  /*! @brief Decodes APDU portion of a packet and returns InvokeID, ServiceChoice and ServiceRequestOffset
   *
   * In this method APDU portion of a received BACnet request is decoded and the provided 
   * refererences to invokeID, serviceChoice and ServiceRequestOffset variables are set.
   * Called in combination with decodeNPDU method (decodeNPDU must be called first).
   * 
   * @param paAPDUOffset Reference to the variable indicating the offset to the APDU portion
   * @param paInvokeID Reference to the variable indicating invoke id of the received request
   * @param paServiceChoice Reference to the variable indication service choice of the received packet
   * @param paServiceRequestOffset Reference to the variable indicating the offset service request portion
   * @return 'true' in case of a success, 'false' otherwise
   *  
   */
  bool decodeAPDU(const TForteUInt16 &paAPDUOffset, TForteUInt8 &paInvokeID, TForteUInt8 &paServiceChoice, TForteUInt16 &paServiceRequestOffset);


  /*! @brief Handles the application part of ReadPorperty service request
   *
   * This method handles a ReadProperty service request by decoding its service request part (uses libbacnet's
   * rp_decode_service_request), getting the right object from the object table (getBacnetObject), and encoding
   * and sending a Complex Ack message with encoded value of the property requested by the ReadProperty service
   * request (obj->ReadProperty).
   * 
   * @param paServiceRequestOffset Integer indicating the offset to the service request portion of a packet
   * @param paServiceRequestLen Integer indication the length of the service request portion of a packet
   * @param paInvokeID Integer indication invoke id of the received service request
   * @param paSourceAddress Struct containing ip address and port of the requesting side
   */
  void handleReadPropertyRequest(const TForteUInt16 &paServiceRequestOffset, TForteUInt16 paServiceRequestLen, const TForteUInt8 &paInvokeID, const sockaddr_in &paSourceAddress);
  
  /*! @brief Handles the application part of WriteProperty service request
   *
   * This method handles a WriteProperty service request by decoding its service request part (uses libbacnet's
   * wp_decode_service_request), getting the right object from the object table (getBacnetObject), performing a
   * write operation to one of the object's properties (obj->writeProperty), notifying its config fb about 
   * the performed write (triggers config FB's external event), and eventually sending a simple acknowledgement 
   * in response.
   * 
   * @param paServiceRequestOffset Integer indicating the offset to the service request portion of a packet
   * @param paServiceRequestLen Integer indication the length of the service request portion of a packet
   * @param paInvokeID Integer indication invoke id of the received service request
   * @param paSourceAddress Struct containing ip address and port of the requesting side
   */
  void handleWritePropertyRequest(const TForteUInt16 &paServiceRequestOffset, TForteUInt16 paServiceRequestLen,  const TForteUInt8 &paInvokeID, const sockaddr_in &paSourceAddress);
  
  /*! @brief Handles the application part of Who-Is service request   
   *
   * This method handles a Who-Is service request by decoding its service request part (uses libbacnet's
   * whois_decode_service_request), comparing own device object id with the ones requested by Who-Is, and
   * eventually sending an I-Am broadcast message in case the device object it is within bounds.
   * 
   * @param paServiceRequestOffset Integer indicating the offset to the service request portion of a packet
   * @param paServiceRequestLen Integer indication the length of the service request portion of a packet
   * @param paSourceAddress Struct containing ip address and port of the requesting side
   */
  void handleWhoIsRequest(const TForteUInt16 &paServiceRequestOffset, TForteUInt16 paServiceRequestLen);
  
  /*! @brief Handles the application part of SubscribeCOV service request  
   *
   * This method handles a SubscribeCOV service request by decoding its service request part (uses libbacnet's
   * cov_subscribe_decode_service_request), performing a look-up for the desired object in COVReporters list, 
   * updating the subscription data in case the object was found, and eventually sending out a simple acknowledgement.
   * 
   * @param paServiceRequestOffset Integer indicating the offset to the service request portion of a packetw
   * @param paInvokeID Integer indication invoke id of the received service request
   * @param paSourceAddress Struct containing ip address and port of the requesting side
   */
  void handleSubscribeCOVRequest(const TForteUInt16 &paServiceRequestOffset, const TForteUInt8 &paInvokeID, sockaddr_in &paSourceAddress);

  /*! @brief Returns a BACnet object from the object table based on the provided type and id
   *
   * In this method, an object look-up based on the provided object type and id is performed by iterating
   * through the server's table of BACnet objects (mObjectTable). If specified object is found, 
   * pointer to it gets returned. Otherwise, NULL is returned. 
   * 
   * @param paObjectType Type of the requested object
   * @param paObjectInstance Integer, indicating object's ID
   * @return Pointer to BACnet object instance, NULL if not found
   */
  CBacnetObject* getBacnetObject(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectInstance);


  /*! @brief Returns an entry from the COVReporters list based on the provided object type and object id
   *
   * In this method, an object look-up based on the provided object type and id is performed by iterating
   * through the server's list objects, configured to send unconfirmed COV notifications (mCOVReporters). If
   * specified object is found, pointer to the list entry is gets returned. Otherwise, NULL is returned. 
   * 
   * @param paObjectType Type of the requested object
   * @param paObjectInstance Integer, indicating object's ID
   * @return Pointer to the COVReporters list entry, NULL if not found
   */
  SCOVReporter* getCOVReportersEntry(BACNET_OBJECT_TYPE paObjectType, TForteUInt32 paObjectInstance);

  /*! @brief Adds a new entry to the provided list of subscribers
   *
   * This method is used for updating the list of subscriptions of a specific entry from the list of COV
   * reporters (mCOVReporters).
   * 
   * @param paSubsList To-be-updated list of subsribers
   * @param paCOVData Data of COV subscription request
   * @param paAddr Source address of the COV subscription request
   * @param paPort Source port number of the COV subscription request
   * @return 'true' if new entry was added, 'false' if subsriber is already in the list 
   */
  bool updateCOVSubscriptionDataList(TSubscriptions *paSubsList, BACNET_SUBSCRIBE_COV_DATA *paCOVData, in_addr *paAddr, TForteUInt16 *paPort); 

  /*! @brief Method for sending out pending COV notifications
   *
   * In this method, every COV reporters list entry (mCOVReporters) is checked for COV condition.
   * In cased the COV contidion of a specific object holds, a COV notification is sent to each subscriber (sendCOVReports).
   */
  void processPendingCOVReports();


  /*! @brief Sends out COV notifications to every subscriber of a given COVReporter
   *
   * In this method, an unconfirmed COV notification gets encoded and sent to every subscriber of a provided 
   * COVReporter entry from the list of objects configured to send out COV notifications (mCOVReporters).
   * 
   * @param paCOVReporter Pointer to mCOVReporters entry
   */
  void sendCOVReports(SCOVReporter *paCOVReporter);

  //! Methods required by CExternalEventHandler, not used
  void enableHandler(void) {};
  void disableHandler(void) {};
  void setPriority(int paPriority){std::ignore = paPriority;}; // supress unused parameter warning
  int getPriority(void) const { return 0; };
  size_t getIdentifier() const { return 0;};

};

#endif