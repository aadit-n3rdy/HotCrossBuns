# HotCrossBuns Protocol
A text-based protocol between client and server

## Publish:

**Publishing a message:**
*Client to node*

```
PUBLISH
<Topic Name>
SIZE <size in bytes>
<binary transmission of message>
```

**Response:**
*Node to client*

This is required since msg ID is required

```
ACK <msg ID allocated>
```

**Confirmation:**
*Node to client*

This is required to implement synchronous send calls

```
SENT <msg ID>
```

## Subscribe:

**Subscribing to topic:**
*Client to node*

```
SUBSCRIBE
<Topic Name>
```

**Message received:**
*Node to client*

```
MSG
<Topic name>
SIZE <size in bytes>
<binary transmission of message>
```
