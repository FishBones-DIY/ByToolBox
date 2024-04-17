#include "log.h"

namespace smart_glog
{
	bool initLogger(const char* module_name, const char* log_path)
	{
		static bool initialized = false;
		if (initialized) 
			return initialized;

		google::InitGoogleLogging(module_name);
		google::SetLogDestination(google::GLOG_ERROR, "");
		google::SetLogDestination(google::GLOG_WARNING, "");
		google::SetStderrLogging(google::GLOG_INFO);

		google::SetLogFilenameExtension("log_");
		FLAGS_log_dir = log_path;
		FLAGS_logtostderr = false;					//TRUE:��׼���,FALSE:�ļ����
		FLAGS_alsologtostderr = true;				//������־�ļ�֮���Ƿ���Ҫ��׼���
		FLAGS_colorlogtostderr = true;				// Set log color
		FLAGS_logbufsecs = 0;						// Set log output speed(s)
		FLAGS_max_log_size = 10;					// Set max log file size
		FLAGS_stop_logging_if_full_disk = true;		// If disk is full

		return initialized = true;
	}

	void shutdownLogger()
	{
		google::ShutdownGoogleLogging();
	}

} // namespace toolbox