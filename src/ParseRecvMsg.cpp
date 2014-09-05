#include "zlib.h"
#include "ParseRecvMsg.h"

using namespace muduo;
using namespace muduo::net;
using namespace hub;
extern boost::scoped_ptr<muduo::AsyncLogging> g_asyncLog;


const char* findSpace(const muduo::net::Buffer &buf, const char* start) const
 {
   assert(buf.peek() <= start);
   assert(start <= buf.beginWrite());
   const void* space = memchr(start, ' ', buf.beginWrite() - start);
   return static_cast<const char*>(space);
 }


// sub <topic>
// unsub <topic>
// pub <topic>
// unpub <topic>
// send <topic> <content>
ParseResult pubsub::parseMessage(Buffer* buf,
                                 string* cmd,
                                 string* topic,
                                 string* content)
{
  const char* space = findSpace(buf, buf->peek());
  if(!space){
	LOG_ERROR << "No space find in buf ";
	return kError;
  }
  
 
 cmd->assign(buf->peek(), space);		//end should be space - 1, test, fix me

 if("send" != cmd && "sub" != cmd 
  	&& "pub" != cmd && "unsub" != cmd && "unpub"){
	LOG_ERROR << "cmd " << cmd << "unkown" << endl;
	return kError;
 }

 if("send" != cmd){
	topic.assign(space + 1, udwLen - sizeof(uint32_t) - cmd.size() - sizeof(' '));
	LOG_DEBUG << "topic " << topic;
 }else{
 	const char *topicEnd = findSpace(buf, buf->peek(buf, space + 1));
	if(!topicEnd){
		LOG_ERROR << "topicEnd NOT found";
		return kError;	
	}

	topic.assign(space + 1, topicEnd);
	LOG_DEBUG << "topic " << topic;

	content.assign(topicEnd + 1, udwLen - sizeof(uint32_t) - cmd.size() - topic.size() - 2 * sizeof(' '));
 }

	return kSuccess;

}

