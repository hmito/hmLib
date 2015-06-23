#ifndef HMLIB_PROPERTIES_PROPERTYCONNECTION_INC
#define HMLIB_PROPERTIES_PROPERTYCONNECTION_INC 100
#
#include<memory>
namespace hmLib {
	namespace properties {
		class connection_client_mixin;
		class connection_host_mixin{
			friend connection_client_mixin;
		private://for weak_property
			std::shared_ptr<int> IsExpired;
		protected:
			connection_host_mixin() {}
		public:
			bool is_expire() { return !static_cast<bool>(IsExpired); }
			void expire() { IsExpired.reset(); }
		};
		class connection_client_mixin {
		private:
			std::weak_ptr<int> IsExpired;
		protected:
			connection_client_mixin() {}
			void client_connect(connection_host_mixin& Host_) {
				if(!Host_.IsExpired)Host_.IsExpired.reset(new int);
				IsExpired=Host_.IsExpired;
			}
		public:
			void disconnect() { IsExpired.reset(); }
			bool is_connect() const{ return !(IsExpired.expired()); }
		};
	}
}
#
#endif
