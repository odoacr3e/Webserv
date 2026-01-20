 #ifndef STATUS_CODES_HPP
# define STATUS_CODES_HPP

enum	e_http_codes
{
//NOTE - https://en.wikipedia.org/wiki/List_of_HTTP_status_codes

//SECTION - informational response: tells the client to wait for a final response
	HTTP_IR_CONTINUE = 100,//Expect: 100-continue is set in request
	HTTP_IR_SWITCH_PROT = 101,//server agrees on changing protocols.
	HTTP_IR_PROCESSING = 102,//(DEPRECATED) server is processing the request
	HTTP_IR_EARLY_HINTS = 103,//return some response headers.
//SECTION - success: the server successfully *accepted* the request
	HTTP_OK = 200,//request fully processed. Everything went ok
	HTTP_OK_CREATED = 201,//request fully processed; new resource created
	HTTP_OK_ACCEPTED = 202,//request NOT fully processed (in progress).
	HTTP_OK_NO_AUTHORITATIVE_INFO = 203,//the proxy received 200 from the origin
	HTTP_OK_NO_CONTENT = 204,//request fully processed; no content returned
	HTTP_OK_RESET_CONTENT = 205,//like 204; requester is asked to reset doc view
	HTTP_OK_PARTIAL_CONTENT = 206,//only a part of the resource is sent (byte serving)
	HTTP_OK_MULTI_STATUS = 207,//XML message with multiple code (one per sub request)
	HTTP_OK_ALREADY_REPORTED = 208,//multi code request already reported this
	HTTP_OK_IM_USED = 226,//like 200; a modified copy of data is returned
//SECTION - redirection: client must take addictional action, 
//			such as URL redirection.
//			for GET/HEAD method, a user agent can handle those actions.
	HTTP_RD_MULTIPLE_CHOICE = 300,//multiple resource that the client may choose
	HTTP_RD_MOVED_PERM = 301,//client must ask the user before redirecting
	HTTP_RD_FOUND = 302,//resource found, but in another URL (Location header)
	HTTP_RD_SEE_OTHER = 303,//like 302; encourage a POST request to the location
	HTTP_RD_NOT_MODIFIED = 304,//answer to if-modified-since header field
	HTTP_RD_USE_PROXY = 305,//you need to use this proxy to get this resource
	HTTP_RD_SWITCH_PROXY = 306,//(DEPRECATED) use this proxy in future calls
	HTTP_RD_TEMP_REDIRECT = 307,//like 303; but with the same method
	HTTP_RD_PERM_REDIRECT = 308,//like 307; but for this and future request; Imposes the use of the same http method on linked http requests
//SECTION - client error: server must give:
//						-	an entity with the explaination of the error;
//						-	specify if the issue is temponary or permanent.
	HTTP_CE_BAD_REQUEST = 400,//syntax, size too large, deceptive routing...
	HTTP_CE_UNATHORIZED = 401,//encourage the user to log in
	HTTP_CE_PAYMENT_REQUIRED = 402,//used by some services when daily usage ended
	HTTP_CE_FORBIDDEN = 403,//resource cannot be processed
	HTTP_CE_NOT_FOUND = 404,//resource not found
	HTTP_CE_METHOD_NOT_ALLOWED = 405,//requested resource needs another method
	HTTP_CE_NOT_ACCEPTABLE = 406,//request resource is incompatible with headers
	HTTP_CE_PROXY_AUTH_REQUIRED = 407,//client must authenticate with the proxy
	HTTP_CE_REQUEST_TIMEOUT = 408,//client did not produced a request in time
	HTTP_CE_CONFLICT = 409, //ex: resource is edited by another process
	HTTP_CE_GONE = 410, //like 404; encourage search engines to remove this
	HTTP_CE_LENGTH_REQUIRED = 411,//request did not specify the length
	HTTP_CE_PRECONDITION_FAIL = 412,//one condition in the header field failed
	HTTP_CE_PAYLOAD_TOO_LARGE = 413,//request is larger by server limits
	HTTP_CE_URI_TOO_LONG = 414,//happens with query string too long
	HTTP_CE_MEDIA_TYPE = 415,//ex: images format not accepted
	HTTP_CE_RANGE_NOT_SATISFABLE = 416,//resource portion requested not available
	HTTP_CE_EXPECTAION_FAIL = 417,// server cannot respect the Expect header
	HTTP_CE_IM_TEAPOT = 418,//april joke: returned by teapots requested to brew coffee
	HTTP_CE_MISDIRECTED = 421,//request directed to an invalid server
	HTTP_CE_CONTENT_UNPROCESSABLE = 422,//request cannot be processed.
	HTTP_CE_LOCKED = 423,//resource is locked
	HTTP_CE_DEPENDENCY_FAIL = 424,//request depended on another request. That failed
	HTTP_CE_TOO_EARLY = 425,//request may be replayed. No processing done
	HTTP_CE_UPGRADE_REQUIRED = 426,//ex: client switching to TLS/1.3
	HTTP_CE_PRECONDITION_REQUIRED = 428,//see 'lost update' problem
	HTTP_CE_TOO_MANY_REQUEST = 429,//too many in a given range of time
	HTTP_CE_HEADER_FIELD_TOO_LARGE = 431,//one or more fields
	HTTP_CE_UNAVAILABLE_FOR_LEGAL_REASON = 451,//it's a reference of Fahreneit 451
//SECTION - server error: server must give:
//						-	an entity with the explaination of the error 
//							(optional for HEAD);
//						-	specify if the issue is temponary or permanent.
	HTTP_SE_INTERNAL = 500,//unexpected condition encountered
	HTTP_SE_NOT_IMPLEMENTED = 501,//method/functionality not supported
	HTTP_SE_BAD_GATEWAY = 502,//proxy received invalid response from origin
	HTTP_SE_SERVICE_UNAVAILABLE = 503,//ex: down for mantainance
	HTTP_SE_TIMEOUT_GATEWAY = 504,//proxy didn't receive invalid response in time
	HTTP_SE_HTTP_VERSION_UNSOPPORTED = 505,
	HTTP_SE_VARIANT_NEGOTIATES = 506,//see 'circular reference'
	HTTP_SE_STORAGE_INSUFFICIENT = 507,//request cannot be represented in mem
	HTTP_SE_LOOP_DETECTED = 508,//infine loop while processing request
	HTTP_SE_NOT_EXTENDED = 510,//server needs further extension to the request to process
	HTTP_SE_NETWORK_AUTH_REQUIRED = 511,//see 'captive portals'
	//unknown
	HTTP_UNKNOWN = 999, //wut?

//NOTE - services like nginx, cloudflare, Amazon, Linkedln use other codes
//		however, they are not official
};

# define 	VALID_HTTP_STR {\
	"OK",\
	"Created",\
	"Accepted",\
	"No Content",\
	"IM Used",\
	"Moved Permanently" ,\
	"Found",\
	"Not Modified",\
	"Temporary Redirect",\
	"Permanent Redirect",\
	"Bad Request",\
	"Unauthorized",\
	"Forbidden",\
	"Not Found",\
	"Method Not Allowed",\
	"Conflict",\
	"Length Required",\
	"Uri Too Long",\
	"Unsupported Media Type",\
	"I'm a Teapot",\
	"Unprocessable Content",\
	"Internal Server Error",\
	"Not Implemented",\
	"HTTP Version Not supported"\
}

# define 	VALID_HTTP_CODES {\
	HTTP_OK,\
	HTTP_OK_CREATED,\
	HTTP_OK_ACCEPTED,\
	HTTP_OK_NO_CONTENT,\
	HTTP_OK_IM_USED,\
	HTTP_RD_MOVED_PERM,\
	HTTP_RD_FOUND,\
	HTTP_RD_NOT_MODIFIED,\
	HTTP_RD_TEMP_REDIRECT,\
	HTTP_RD_PERM_REDIRECT,\
	HTTP_CE_BAD_REQUEST,\
	HTTP_CE_UNATHORIZED,\
	HTTP_CE_FORBIDDEN,\
	HTTP_CE_NOT_FOUND,\
	HTTP_CE_METHOD_NOT_ALLOWED,\
	HTTP_CE_CONFLICT,\
	HTTP_CE_LENGTH_REQUIRED,\
	HTTP_CE_URI_TOO_LONG,\
	HTTP_CE_MEDIA_TYPE,\
	HTTP_CE_IM_TEAPOT,\
	HTTP_CE_CONTENT_UNPROCESSABLE,\
	HTTP_SE_INTERNAL,\
	HTTP_SE_NOT_IMPLEMENTED,\
	HTTP_SE_HTTP_VERSION_UNSOPPORTED,\
	HTTP_UNKNOWN\
}

#endif