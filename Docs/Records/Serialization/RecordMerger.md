# Record Merger
Record Merger is multithreaded record reading, merging and writing engine. 
It ensures safety of asynchronous multithreaded operations on Records' binary data.

Record Merger contains a **fixed** pool of [RecordObjects](./RecordObject.h), each containing a growing arena for IR nodes, a builder and metadata.
Size of pool can be regulated automatically by the engine when nothing is locking mutex(no reading or writing operations).

Synchronization mechanism works automatically to use free RecordObject in the pool without thread providing any information.
Synchronization is completely thread-agnostic, it only triggers if multiple operations are performed at the same time.

If at one point [RecordSource](./RecordSource.md) contains [SerialRecordFlag::Deleted](./SerialRecordFlag.md), 
record is considered deleted and null result is returned immediately.