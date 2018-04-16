#ifndef IPV4SUBNETTING
#define IPV4SUBNETTING
#include <array>

using std::string;
using arrayInt  = std::array<int, 4>;

class Address {
    private:
        arrayInt ipv4_address;                   // IPv4 address
        arrayInt cidrmask;                       // CIDR mask
        arrayInt subnetmask;                     // Subnet mask 
        int cidrmask_length;                     // prefix length of cidr mask
        int subnetmask_length;                   // prefix length of subnet mask
        int  num_of_subnets;                     // Number of subnets per network
        int  num_of_hosts;                       // Number of hosts per subnet 
        int  num_of_host_bits;                   // Number of host bits
        int  num_of_subnet_bits;                 // Number of subnet bits
        int  num_of_network_bits;                // Number of network bits
        char network_class;                      // Network type (A, B, C, D, E)
    public:
        Address();
        ~Address();
        arrayInt getIPv4Address()   { return ipv4_address; }
        arrayInt getCidrMask()      { return cidrmask; }
        arrayInt getSubnetMask()    { return subnetmask; }
        int  getNumOfSubs()         { return num_of_subnets; }
        int  getNumOfHosts()        { return num_of_hosts; }
        char getNetClass()          { return network_class; }

        arrayInt fromString(const string &address);
        arrayInt findResidentSubnet(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask);
        arrayInt prefixToDDN(const int &prefix);
        string toString(const arrayInt &address);

        int  findInterestingOctet(const arrayInt &subnetmask);
        int  findSubIncNum(const arrayInt &subnetmask, const arrayInt &cidrmask);
        int  findPrefix(const arrayInt &mask);
        int  findNumOfHosts(const arrayInt &subnetmask);
        int  findNumOfHosts(const int &prefix);
        int  findNumOfSubs(const arrayInt &subnetmask, const arrayInt &cidrmask);
        int  findNumOfSubs(const int &subnetprefix, const int &cidrprefix);

        void findNetworkClass(const arrayInt &address, bool defaultmask = 0, char netclass = '0');
        void findAllMasks(const char networkclass, const int subnets, const int hosts);
        void subnetDetails(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask);
        void generateSubnets(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask, const bool verbose);

        bool isValidAddr(const string &address, const string &masktype);
        bool isValidIP(const string &address);
        bool isValidMask(const string &mask, const string &masktype);
        bool isValidDesign(const char *input, const string &type);

        void printAddress(const arrayInt &address);
        void printUsage();
        void tableFormat(string str);
};
#endif
