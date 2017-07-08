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
 *******************************************************************************/

#include "MQTTPacket.h"
#include "MQTTConnect.h"
#include "ESP8266.h"

#include <string.h>

/**
  * Determines the length of the MQTT connect packet that would be produced using the supplied connect options.
  * @param options the options to be used to build the connect packet
  * @return the length of buffer needed to contain the serialized version of the packet
  */
   
int MQTTSerialize_connectLength(MQTTPacket_connectData* options)
{
	int len = 0;


	if (options->MQTTVersion == 3)
		len = 12; 
	else if (options->MQTTVersion == 4)
		len = 10;

	len += MQTTstrlen(options->clientID)+2;
	if (options->willFlag)
		len += MQTTstrlen(options->will.topicName)+2 + MQTTstrlen(options->will.message)+2;
	if (options->username.cstring || options->username.lenstring.Data)
		len += MQTTstrlen(options->username)+2;
	if (options->password.cstring || options->password.lenstring.Data)
		len += MQTTstrlen(options->password)+2;

	return len;
}	


		   		  
int MQTTSerialize_connect(unsigned char* buf, int buflen, MQTTPacket_connectData* options)
{
	unsigned char *ptr = buf;  
	MQTTHeader header = {0};
	MQTTConnectFlags flags = {0};
	int len = 0;
	int rc = -1;

	header.byte = 0;
	header.bits.type = CONNECT_MSG;
	Serial0_SendChar(header.byte); 
   
	ptr += MQTTPacket_encode(ptr, len); 

	if (options->MQTTVersion == 4)
	{
		Serial0_SendString("MQTT");
		Serial0_SendChar((char) 4);
	}
	else
	{
		Serial0_SendString("MQIsdp");
		Serial0_SendChar((char) 3);
	}

	flags.all = 0;
	flags.bits.cleansession = options->cleansession;
	flags.bits.will = (options->willFlag) ? 1 : 0;
	if (flags.bits.will)
	{
		flags.bits.willQoS = options->will.qos;
		flags.bits.willRetain = options->will.retained;
	}

	if (options->username.cstring || options->username.lenstring.Data)
		flags.bits.username = 1;
	if (options->password.cstring || options->password.lenstring.Data)
		flags.bits.password = 1;

	Serial0_SendChar(flags.all);
	Serial0_SendChar(options->keepAliveInterval);
	Serial0_SendString1(options->clientID.lenstring.Data);
	if (options->willFlag)
	{
		Serial0_SendString1(options->will.topicName.lenstring.Data);
		Serial0_SendString1( options->will.message.lenstring.Data);
	}
	if (flags.bits.username)
		Serial0_SendString1( options->username.lenstring.Data);
	if (flags.bits.password)
		Serial0_SendString1(options->password.lenstring.Data);

	rc = ptr - buf;

	exit:
	return rc;
}					  