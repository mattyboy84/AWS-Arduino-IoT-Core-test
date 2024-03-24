#define SECRET_SSID "WiFi Name"
#define SECRET_PASS "WiFi Passowrd"

#define SECRET_BROKER "broker-ats.iot.{AWS::Region}.amazonaws.com"

// Device Certificate
const char SECRET_CERTIFICATE[] = R"(
-----BEGIN CERTIFICATE-----
{Certificate}
-----END CERTIFICATE-----
)";

// Device Private Key
const char AWS_CERT_PRIVATE[] = R"(
-----BEGIN RSA PRIVATE KEY-----
{Device Private Key}
-----END RSA PRIVATE KEY-----
)";
