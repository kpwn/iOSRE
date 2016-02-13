# Mach & BSD

The Mach microkernel and the BSD kernel are the two major components of XNU.
<br>
Mach implements the absolute core of the operating system, while the BSD layer is built on top of Mach, and implements higher level concepts.

## Mach
### Introduction
Mach is a microkernel, originally designed by CMU between the 80s and the 90s. Apple chose this microkernel as the base of their new operating system, and has substantially modified it from the original version.

Mach is the absolute foundation of XNU, it deals with every crucial aspect needed for the correct functioning of the operating system. Mach does not bother with higher level concepts such as file systems or device drivers, for example. These concepts are left for other components to implement.

### Philosophy
As stated before, Mach does not concern itself with higher level concepts, implementing just the bare-bones of the operating system. Mach is specifically designed to allow an entire operating system to be built upon it, which is exactly what Apple has done.

The core idea behind Mach its minimalism. Being a microkernel, Mach implements only the fundamental abstractions (described more in detail later).

Another key point in the Mach philosophy is how the various subsystems communicate, i.e. via a message passing facility. Interaction between components is completely implemented via IPC in Mach. This means that objects cannot directly call/invoke themselves, rather a message is sent from one object to another, that message is queued until the receiver handles it.
This may seem unconventional, and it is. But, since Mach is a microkernel, all of the subsystems are separated from one another, thus message passing is needed for communication.

Summarizing, remember that Mach is just the pure foundation of XNU, and everything else is built upon it.

### Implementation
#### Execution primitives
###### Threads
Mach is responsible for the basics abstractions of execution.
<br>
The most basic unit of execution is indeed a _thread_. A thread provides basics scheduling statistics and machine registers, used for execution. Remember that a thread has no links to resources (such as virtual memory), as these are implemented in _tasks_.
<br>
Also remember that exist a direct link between a Mach thread and a BSD thread.

###### Tasks
A _task_ is basically a container object. It implements resources, such as virtual memory, and its only purpose it's to contain threads, since a task has no life by its own.
<br>
It also exist a direct map between Mach tasks and BSD processes, as with threads.
<br>
The threads in a task are kept in a singly linked list.

#### IPC primitives
###### Messages
Mach messages are simply data exchanged between two endpoints, called ports. Mach messages constitutes the basic building block for IPC communication.

A message has an header (`mach_msg_header_t`), a body (`mach_msg_body_t`), and optionally a trailer (`mach_msg_trailer_t`).

The **header** contains all informations needed for the message to be sent and correctly received, such as:

-   `msgh_size`: The message size.
-   `msgh_remote_port`: The destination Mach port.
-   `msgh_local_port`: The local Mach port.
-   `msgh_id`: An unique ID. Used specifically with MIG.
-   `msgh_bits`: Optional bits.

A message is deemed _complex_ if the `MACH_MSGH_BITS_COMPLEX` flag is present in the `msgh_bits` field in its header. A complex message is structured differently: After the header, it follows a descriptor count field, followed by actual descriptors.
<br>
There are various types of descriptors, they generally serve the purpose of including special "attachments" to the message (for example `MACH_MSG_PORT_DESCRIPTOR` a descriptor for attaching a Mach port, or `MACH_MSG_OOL_DESCRIPTOR` for out-of-line data, i.e. pointers to data external to the message).

The standard **body** contains simply a `mach_msg_size_t`. The body is the actual data of the message and can contain virtually anything.

The **trailer** contains a `mach_msg_trailer_type_t` (`unsigned int`), which specifies the trailer type, and a `mach_msg_trailer_size_t` for specifying the trailer size.

###### Ports
Mach ports are endpoints for communication. From user-land perspective, a Mach port is nothing more than an integer, an opaque handle to a more complex object held in-kernel.

Ports implement message queues to receive and enqueue messages. A message remains in a queue until a receiver dequeues it. Messages sent are guaranteed to be delivered.

Ports may be accessed only via port rights. Those are basically permissions held by threads and tasks, needed to interact with ports. Here's a list of all the port rights:

-   `MACH_PORT_RIGHT_SEND`: Whoever holds this right for a specific port, is allowed to send (enqueue) messages to that port. More than one entity at a time are allowed to retain this right.
-   `MACH_PORT_RIGHT_RECEIVE`: Whoever holds this right for a specific port, is allowed to receive (dequeue) messages from that port. Holding this right corresponds to having ownership over the port. Only one entity at a time is allowed to retain this right.
-   `MACH_PORT_RIGHT_SEND_ONCE`: Akin to `MACH_PORT_RIGHT_SEND`, but the right is revoked after one message has been sent.
-   `MACH_PORT_RIGHT_PORT_SET`: Receive rights for multiple ports (a port set).

## BSD
BSD is primarily needed to implement what Mach leaves unimplemented. Things such as users, groups, files and others are obviously needed. BSD has also been heavily modified by Apple and differs significantly from other BSD implementations.

Although Mach builds the foundations, XNU can still be considered mostly a BSD-like system, since the POSIX APIs exposed are more complete and effectively more used than Mach's.
