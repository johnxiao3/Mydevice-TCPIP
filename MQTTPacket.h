/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Xiang Rong - 442039 Add makefile to Embedded C client
 *******************************************************************************/

#ifndef MQTTPACKET_H_
#define MQTTPACKET_H_

enum errors
{
	MQTTPACKET_BUFFER_TOO_SHORT = -2,
	MQTTPACKET_READ_ERROR = -1,
	MQTTPACKET_READ_COMPLETE
};

enum msgTypes
{
	CONNECT_MSG = 1, CONNACK_MSG, PUBLISH_MSG, PUBACK_MSG, PUBREC_MSG, PUBREL_MSG,
	PUBCOMP_MSG, SUBSCRIBE_MSG, SUBACK_MSG, UNSUBSCRIBE_MSG, UNSUBACK_MSG,
	PINGREQ_MSG, PINGRESP_MSG, DISCONNECT_MSG
};

typedef struct
{
	/** The eyecatcher for this structure.  must be MQTC. */
	const char struct_id[4];
	/** The version number of this structure.  Must be 0, 1, 2, 3 or 4.  
	 * 0 signifies no SSL options and no serverURIs
	 * 1 signifies no serverURIs 
	 * 2 signifies no MQTTVersion
	 * 3 signifies no returned values
	 */
	int struct_version;
	/** The "keep alive" interval, measured in seconds, defines the maximum time
   * that should pass without communication between the client and the server
   * The client will ensure that at least one message travels across the
   * network within each keep alive period.  In the absence of a data-related 
	 * message during the time period, the client sends a very small MQTT 
   * "ping" message, which the server will acknowledge. The keep alive 
   * interval enables the client to detect when the server is no longer 
	 * available without having to wait for the long TCP/IP timeout.
	 */
	int keepAliveInterval;
	/** 
   * This is a boolean value. The cleansession setting controls the behaviour
   * of both the client and the server at connection and disconnection time.
   * The client and server both maintain session state information. This
   * information is used to ensure "at least once" and "exactly once"
   * delivery, and "exactly once" receipt of messages. Session state also
   * includes subscriptions created by an MQTT client. You can choose to
   * maintain or discard state information between sessions. 
   *
   * When cleansession is true, the state information is discarded at 
   * connect and disconnect. Setting cleansession to false keeps the state 
   * information. When you connect an MQTT client application with 
   * MQTTClient_connect(), the client identifies the connection using the 
   * client identifier and the address of the server. The server checks 
   * whether session information for this client
   * has been saved from a previous connection to the server. If a previous 
   * session still exists, and cleansession=true, then the previous session 
   * information at the client and server is cleared. If cleansession=false,
   * the previous session is resumed. If no previous session exists, a new
   * session is started.
	 */
	int cleansession;
	/** 
   * This is a boolean value that controls how many messages can be in-flight
   * simultaneously. Setting <i>reliable</i> to true means that a published 
   * message must be completed (acknowledgements received) before another
   * can be sent. Attempts to publish additional messages receive an
   * ::MQTTCLIENT_MAX_MESSAGES_INFLIGHT return code. Setting this flag to
	 * false allows up to 10 messages to be in-flight. This can increase 
   * overall throughput in some circumstances.
	 */
	int reliable;		

	/** 
   * MQTT servers that support the MQTT v3.1.1 protocol provide authentication
   * and authorisation by user name and password. This is the user name 
   * parameter. 
   */
	const char* username;	
	/** 
   * MQTT servers that support the MQTT v3.1.1 protocol provide authentication
   * and authorisation by user name and password. This is the password 
   * parameter.
   */
	const char* password;
	/**
   * The time interval in seconds to allow a connect to complete.
   */
	int connectTimeout;
	/**
	 * The time interval in seconds
	 */
	int retryInterval;
	/**
	 * The number of entries in the optional serverURIs array. Defaults to 0.
	 */
	int serverURIcount;
	/**
	 * Sets the version of MQTT to be used on the connect.
	 * MQTTVERSION_DEFAULT (0) = default: start with 3.1.1, and if that fails, fall back to 3.1
	 * MQTTVERSION_3_1 (3) = only try version 3.1
	 * MQTTVERSION_3_1_1 (4) = only try version 3.1.1
	 */
	int MQTTVersion;
	/**
	 * Returned from the connect when the MQTT version used to connect is 3.1.1
	 */
} MQTTClient_connectOptions;

/**
 * Bitfields for the MQTT header byte.
 */
typedef union
{
	unsigned char byte;	                /**< the whole byte */
#if defined(REVERSED)
	struct
	{
		unsigned int type : 4;			/**< message type nibble */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int retain : 1;		/**< retained flag bit */
	} bits;
#else
	struct
	{
		unsigned int retain : 1;		/**< retained flag bit */
		unsigned int qos : 2;				/**< QoS value, 0, 1 or 2 */
		unsigned int dup : 1;				/**< DUP flag bit */
		unsigned int type : 4;			/**< message type nibble */
	} bits;
#endif
} MQTTHeader;


typedef union
{
	unsigned char all;	/**< all connect flags */
#if defined(REVERSED)
	struct
	{
		unsigned int username : 1;			/**< 3.1 user name */
		unsigned int password : 1; 			/**< 3.1 password */
		unsigned int willRetain : 1;		/**< will retain setting */
		unsigned int willQoS : 2;				/**< will QoS value */
		unsigned int will : 1;			    /**< will flag */
		unsigned int cleansession : 1;	  /**< clean session flag */
		unsigned int : 1;	  	          /**< unused */
	} bits;
#else
	struct
	{
		unsigned int : 1;	     					/**< unused */
		unsigned int cleansession : 1;	  /**< cleansession flag */
		unsigned int will : 1;			    /**< will flag */
		unsigned int willQoS : 2;				/**< will QoS value */
		unsigned int willRetain : 1;		/**< will retain setting */
		unsigned int password : 1; 			/**< 3.1 password */
		unsigned int username : 1;			/**< 3.1 user name */
	} bits;
#endif
} MQTTConnectFlags;	/**< connect flags byte */

	 
typedef struct
{
	int len;
	char* Data;
} MQTTLenString;
 
typedef struct
{
	char cstring;
	MQTTLenString lenstring;
} MQTTString; 

#define MQTTString_initializer {NULL, {0, NULL}}

int MQTTstrlen(MQTTString mqttstring);

typedef struct {
	int (*getfn)(void *, unsigned char*, int); /* must return -1 for error, 0 for call again, or the number of bytes read */
	void *sck;	/* pointer to whatever the system may use to identify the transport */
	int multiplier;
	int rem_len;
	int len;
	char state;
}MQTTTransport;

int MQTTPacket_encode(unsigned char* buf, int length);

void MQTT_Connet();
