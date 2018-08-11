#include "https.h"
#include <curl/curl.h>
#include <iostream>


https::https()
{
}


https::~https()
{
}

void https::post(std::string payload, const char* token) {
	CURL *curl;
	CURLcode res;

	curl_global_init(CURL_GLOBAL_DEFAULT);

	curl = curl_easy_init();
	if (curl) {

		struct curl_slist *header = NULL;
		char auth[256]; //TODO: fix me
		strcpy(auth, "Authorization:Bot ");
		strcat(auth, token);
		header = curl_slist_append(header, auth);
		header = curl_slist_append(header, "User-Agent:roCORD (https://github.com/Normynator/Ragnarok, v1)");
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header);


		curl_easy_setopt(curl, CURLOPT_URL, "https://discordapp.com/api/v6/channels/280871866669989888/messages");

		char content[256]; // TODO: fixme;
		strcpy(content, "content=");
		strcat(content, payload.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, content);
		curl_easy_setopt(curl, CURLOPT_CAINFO, "path/to/cacert.pem");

#ifdef SKIP_PEER_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		/* always cleanup */
		curl_easy_cleanup(curl);
	}

	curl_global_cleanup();
}
