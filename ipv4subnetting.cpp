#include <iostream>
#include <math.h>
#include <bitset>
#include <iomanip>
#include "ipv4subnetting.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::setw;
using std::left;
using std::setfill;
using std::bitset;

// Print usage information 
void Address::printUsage() {
    cout << "Usage: subcal info --ip4 <ipv4address/netmask>  for classful network\n" 
            "       subcal info --ip4 <ipv4address/cidrmask> <subnetmask> for classless network\n" 
            "       subcal gen  --ip4 <ipv4address/netmask>  for classful subnetting\n"
            "       subcal gen  --ip4 <ipv4address/cidrmask> <subnetmask> for classless subnetting\n"
            "       subcal design  -s <#subnets> -h <#hosts> -c <networkclass>\n\n"
            "Manditory Options\n"
            "\tinfo   \tFind information about classful/classless network\n"
            "\tgen    \tGenerate subnets of classful/classless network\n"
            "\tdesign \tCalculate all possible masks that meet network design\n\n"
            "Options\n"
            "\t--ip4         \tSpecify the ipv4 address\n"
            "\t-v, --verbose \tGenerate all possible subnets used with 'gen'\n"
            "\t-s, --subnets \tSpecify the number of subnets required by design\n"
            "\t-h, --hosts   \tSpecify the number of hosts required by design\n"
            "\t-c, --class   \tSpecify the network class type for the design\n\n"
            "Examples\n"
            "Get some information about specific network/subnet\n"
            "\tsubcal info --ip4 172.16.23.122/24\n"
            "\tsubcal info --ip4 172.16.23.122/24 /26\n"
            "\tsubcal info --ip4 172.16.23.122/255.255.255.0\n"
            "\tsubcal info --ip4 172.16.23.122/255.255.255.0 255.255.255.128\n"
            "Generate all possible subnets of a classful network\n"
            "\tsubcal gen --ip4 192.168.10.10/28\n"
            "\tsubcal gen --ip4 192.168.10.10/255.255.255.240\n"
            "Generate all possible subnets of a classless network\n"
            "\tsubcal gen --ip4 192.168.10.10/24 /29\n"
            "\tsubcal gen --ip4 192.168.10.10/255.255.255.0 /29\n"
            "Note: By default 'subcal gen' will generates only the first 20 subnets, "
            "to generate all possible subnets use '-v'.\n";
}
//Format the output of the generated subnets
void Address::tableFormat(string str) {
    if (str == "header") {
        cout << setfill('-') << setw(75) << '-' << setfill(' ') << endl;
        cout << setw(17) << left << "   Subnet" 
             << setw(14) << left << '|' 
             << setw(24) << left << "Host Range" 
             << setw(4) << left << '|' 
             << setw(15) << "Broadcast" << '|' 
             << endl;
    }
    else if (str == "top" || str == "bottom") {
        cout << setfill('-') << setw(17) << '-' << '+'
             << setfill('-') << setw(37) << '-' << '+' 
             << setfill('-') << setw(18) << '-' << '+' 
             << setfill(' ') << endl;
    }
}
//Initialize the Address private members to zeros 
Address::Address() {
    ipv4_address={{0,0,0,0}};
    cidrmask={{0,0,0,0}};
    subnetmask={{0,0,0,0}};
    cidrmask_length = 0;
    subnetmask_length = 0;
    num_of_subnets = 0;
    num_of_hosts = 0;
    num_of_host_bits = 0;
    num_of_subnet_bits = 0;
    num_of_network_bits = 0;
}
Address::~Address() {
}
// Print ipv4/mask address in DDN format. 
void Address::printAddress(const arrayInt &address){
    for (int i = 0; i < 4; i++){
        if (i < 3)
            cout << address.at(i) << '.';
        else
            cout << address.at(i) << endl;
    } 
}
// Network Design, find all possible mask that meets the design requirements.
void Address::findAllMasks(const char networkclass, const int subnets, const int hosts) {
    findNetworkClass({{0,0,0,0}}, 1, networkclass);
    for (int i = 0; i <= 24; i++)
        if (pow(2, i) >= subnets) {
            num_of_subnets = pow(2, i);
            num_of_subnet_bits = i;
            break;
        }
    for (int i = 0; i <= 24; i++)
        if (pow(2, i)-2 >= hosts) {
            num_of_hosts = pow(2, i) - 2;
            num_of_host_bits = i;
            break;
        }
    int prefix_min = num_of_network_bits + num_of_subnet_bits;   // min prefix that gives max hosts and min subs
    int prefix_max = 32 - num_of_host_bits;   // max prefix that gives max subs and min hosts
    const int pSIZE = prefix_max - prefix_min + 1;
    if (prefix_min > 32 || pSIZE <= 0) {
        cout << "Network Class '"<< networkclass << "' cannot produce " 
             << subnets << " subnets and " << hosts <<" hosts. Try different Class.\n";
        exit(0);
    }
    std::pair<int,string> all_prefixes[pSIZE];
    for (int i = 0; i < pSIZE; i++) {
        all_prefixes[i] = std::make_pair(prefix_min + i, toString(prefixToDDN(prefix_min+i)));
    }
    for (auto i : all_prefixes){
        cout << i.second << " (/" << i.first << "), " 
             << "#subnets: " << findNumOfSubs(i.first, num_of_network_bits)
             << ", #hosts: " << findNumOfHosts(i.first) << endl;
    }
    cout << "Total: " << pSIZE << " possible subnet masks.\n";
}
// Convert /24 to 255.255.255.0 
arrayInt Address::prefixToDDN(const int &prefix){
    arrayInt ddnMask {{0,0,0,0}};
    for (auto i = 0; i < prefix; i++){
        if (i < 8) {
            ddnMask.at(0) += pow(2, 7-i%8);
        }
        else if (i >= 8 && i < 16) {
            ddnMask.at(1) += pow(2, 7-i%8);
        }
        else if (i >= 16 && i < 24) {
            ddnMask.at(2) += pow(2, 7-i%8);
        }
        else if (i >= 24 && i < 32) {
            ddnMask.at(3) += pow(2, 7-i%8);
        }
        else {
            cerr << "ERROR!: Invalid prefix number./n";
            exit(1);
        }
    }
    return ddnMask;
}
// Convert ipv4/mask array to string
string Address::toString(const arrayInt &address){
    string strOct1 = std::to_string(address.at(0));
    string strOct2 = std::to_string(address.at(1));
    string strOct3 = std::to_string(address.at(2));
    string strOct4 = std::to_string(address.at(3));
    string strAddress = strOct1 + '.' + strOct2 + '.' + strOct3 + '.' + strOct4;
    return strAddress;
}
// Find Information about network/subnet
void Address::subnetDetails(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask){
    if (cidrmask_length > subnetmask_length){
        cerr << "ERROR!: Subnet mask must be >= CIDR/Network mask.\n";
        exit(1);
    }
    arrayInt residentSubnet = findResidentSubnet(address, subnetmask, cidrmask);
    arrayInt lastIP = {{0, 0, 0, 0}};
    arrayInt firstIP = {{residentSubnet.at(0), residentSubnet.at(1), residentSubnet.at(2), residentSubnet.at(3)+1}};
    lastIP.at(0) = residentSubnet.at(0) + 255 - subnetmask.at(1);
    lastIP.at(1) = residentSubnet.at(1) + 255 - subnetmask.at(1);
    lastIP.at(2) = residentSubnet.at(2) + 255 - subnetmask.at(2);
    lastIP.at(3) = residentSubnet.at(3) + 254 - subnetmask.at(3);
    arrayInt broadcastAddress = {{lastIP.at(0), lastIP.at(1), lastIP.at(2), lastIP.at(3)+1}};
    findNetworkClass(address);
    cout << setw(25) << left << "\nSubnet Address:" << toString(residentSubnet)
         << setw(25) << left << "\nHost Range:" << toString(firstIP)+" - "+toString(lastIP)
         << setw(25) << left << "\nBroadcast Address:" << toString(broadcastAddress)
         << setw(25) << left << "\nCIDR Mask:" << toString(cidrmask) << " (/" << findPrefix(cidrmask) << ')'
         << setw(25) << left << "\nSubnet Mask:" << toString(subnetmask) << " (/" << findPrefix(subnetmask) << ')'
         << setw(25) << left << "\nSubnet Size:" << findNumOfHosts(subnetmask)
         << setw(25) << left << "\nNum of Subnets:" << findNumOfSubs(subnetmask, cidrmask)
         << setw(25) << left << "\nNetwork Class:" << "Class " << network_class
         << setw(25) << left << "\nSubnet Mask Bits:" << "TODO:"
         << endl;
}
// Generate all possible subnets, host range, brodcast .... of a classful or classless network
void Address::generateSubnets(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask, const bool verbose) {
    if (cidrmask_length > subnetmask_length){
        cerr << "ERROR!: Subnet mask must be >= CIDR mask.\n";
        exit(1);
    }
    int X = 254, Y = 1, Z = 1;           // fix negative value of host 3rd octet if prefix = 32
    if (subnetmask_length == 32){
        X = 255;
        Y = 0;
        Z = 0;
    }
    arrayInt firstSubnet =  findResidentSubnet(address, cidrmask, cidrmask);
    arrayInt subnetAddress {{0,0,0,0}};
    arrayInt firstHostAddr {{0,0,0,0}};
    arrayInt lastHostAddr {{0,0,0,0}};
    arrayInt broadcastAddr {{0,0,0,0}};
    tableFormat("header");
    tableFormat("top");
    int count_subnets = 0;
    for (auto i = 0; i <= subnetmask.at(0) - cidrmask.at(0); i += 256 - subnetmask.at(0)){
        for (auto j = 0; j <= subnetmask.at(1) - cidrmask.at(1); j += 256 - subnetmask.at(1)){
            for (auto k = 0; k <= subnetmask.at(2) - cidrmask.at(2); k += 256 - subnetmask.at(2)) {
                for (auto m = 0; m <= subnetmask.at(3) - cidrmask.at(3); m += 256 - subnetmask.at(3)){
                    subnetAddress.at(0) = firstSubnet.at(0) + i;
                    subnetAddress.at(1) = firstSubnet.at(1) + j;
                    subnetAddress.at(2) = firstSubnet.at(2) + k;
                    subnetAddress.at(3) = firstSubnet.at(3) + m;
                    lastHostAddr.at(0) = subnetAddress.at(0) + 255 - subnetmask.at(0);
                    lastHostAddr.at(1) = subnetAddress.at(1) + 255 - subnetmask.at(1);
                    lastHostAddr.at(2) = subnetAddress.at(2) + 255 - subnetmask.at(2);
                    lastHostAddr.at(3) = subnetAddress.at(3) + X - subnetmask.at(3);
                    firstHostAddr = {{subnetAddress.at(0), subnetAddress.at(1), subnetAddress.at(2), subnetAddress.at(3)+Y}};
                    broadcastAddr = {{lastHostAddr.at(0), lastHostAddr.at(1), lastHostAddr.at(2), lastHostAddr.at(3)+Z}};
                    cout << setw(17) << left << toString(subnetAddress)
                         << setw(2) << left << '|' << setw(16) << left <<  toString(firstHostAddr)
                         << setw(20) << left << "-  "+toString(lastHostAddr)
                         << setw(2) << left << '|' << setw(17) << left << toString(broadcastAddr) << "|\n";
                    ++count_subnets;
                    if ( !verbose && count_subnets > 19 ) {
                        cout << "Use '-v' to generate all possible subnets(" 
                            << findNumOfSubs(subnetmask,cidrmask) <<").\n";
                        goto exit_loops;
                    }
                }
            }
        }
    }
    tableFormat("bottom");
    exit_loops:; 
}
// Validate IPv4 address
bool Address::isValidIP(const string &address) {
    arrayInt ipAddr = fromString(address);
    int found = address.find_first_not_of(".0123456789");      // isdigit returns -1 if no match
    for(auto &i : ipAddr){
        if (i > 255 || i < 0 || found != -1 || ipAddr.size() != 4){
            cerr << "ERROR!: Bad IPv4 address '" << address <<"'.\n";
            exit(1);
        }
    }
    ipv4_address = ipAddr;
    return true;
}
// Validate Subnet/CIDR mask
bool Address::isValidMask(const string &mask, const string &masktype) {
    string inetmask = mask;
    if (inetmask.length() > 7 && inetmask.compare(0,1,"/") == 0) {
        inetmask = mask.substr(1);
    } 
    if(inetmask.compare(0,1,"/") == 0 && inetmask.length() <= 3) {
        int found = inetmask.find_first_not_of("0123456789/");
        int prefix = atoi(inetmask.substr(1).c_str());
        if (inetmask.size() > 3 || prefix < 1 || prefix > 32 || found != -1){
           cerr << "ERROR!: Invalid mask '" << inetmask <<"'.\n";
           exit(1);
        }
        else if (masktype == "cidrmask"){
            cidrmask = prefixToDDN(prefix);
            cidrmask_length = prefix;
        }
        else {
            subnetmask = prefixToDDN(prefix);
            subnetmask_length = prefix;
        }
    }
    else {
        arrayInt ddnmask = fromString(inetmask);
        bool mFlag = 0;   // set if there is an interleaving of 1s and 0s         
        for(size_t i = 0; i < ddnmask.size(); i++){
            if (ddnmask.at(i) != 255 && i < 3 && ddnmask.at(i+1) != 0){
                mFlag = 1; 
            } 
            if ((ddnmask.at(i) != 0 && ddnmask.at(i) != 128 && ddnmask.at(i) != 192 && ddnmask.at(i) != 224
                && ddnmask.at(i) != 240 && ddnmask.at(i) != 248 && ddnmask.at(i) != 252 
                && ddnmask.at(i) != 254 && ddnmask.at(i) != 255) || (mFlag> 0)){
                cerr << "ERROR!: Bad mask '" << inetmask <<"'.\n";
                exit(1);
            }
        }
        if (masktype == "cidrmask"){
            cidrmask = ddnmask;
            cidrmask_length = findPrefix(ddnmask);
        }
        else {
            subnetmask = ddnmask;
            subnetmask_length = findPrefix(ddnmask);
        }
    }
    return true;
}
// Validate address in this format 192.168.10.1/24 or 192.168.10.1/255.255.255.0 
bool Address::isValidAddr(const string &address, const string &masktype) {
    int slash = address.find("/");
    if (slash == -1) {
        cerr << "ERROR!: ipv4 address missing netmask.'" << address <<"'\n";
        cout << "Try, 'subcal --help' for more information.\n";
        exit(1);
    }
    string ipv4address = address.substr(0,address.find("/"));
    string netmask = address.substr(address.find("/"));
    isValidIP(ipv4address);
    if (masktype == "subnetmask")
        isValidMask(netmask, "subnetmask");
    else
        isValidMask(netmask, "cidrmask");
    return true;
}
// Validate the Subnet design options num of hosts, num of subnets and network class 
bool Address::isValidDesign(const char *input, const string &type) {
    int validate_input = string(input).find_first_not_of("0123456789");
    int validate_input2= string(input).find_first_not_of("ABCDE");
    if (type == "subnets") {
       if (validate_input != -1) {
           cerr << "ERROR!: Subnets value must be an integer number.\n";
           exit(1);
       }
       else {
           num_of_subnets = atoi(input);
       }
    }
    else if (type == "hosts") {
       if (validate_input != -1) {
           cerr << "ERROR!: Hosts value must be an integer number.\n";
           exit(1);
       }
       else {
           num_of_hosts = atoi(input);
       }
    }
    else if (type == "netclass") {
       if (validate_input2 != -1 && string(input).size() > 1) {
           cerr << "ERROR!: [Network Class] expected A, B or C.\n";
           exit(1);
       }
       else {
           network_class = input[0];
       }
    }
return true;
}
// Given subnetmask, find number of hosts/subnet 
int Address::findNumOfHosts(const arrayInt &subnetmask) {
     int prefix = findPrefix(subnetmask);
     int val = (prefix == 32) ? 0 : 2;
    return  pow(2, 32 - prefix) - val;
}
// Given prefix, find number of hosts/subnet
int Address::findNumOfHosts(const int &prefix) {
    return  pow(2, 32 - prefix) - 2;
}
// Given subnetmask w/ or w/o cidrmask
int Address::findNumOfSubs(const arrayInt &subnetmask, const arrayInt &cidrmask) {
    int subnetprefix = findPrefix(subnetmask);
    int cidrprefix = findPrefix(cidrmask);
    return pow(2, subnetprefix - cidrprefix);
}
// Given subnet prefix w/ or w/0 cidrmask prefix
int Address::findNumOfSubs(const int &subnetprefix, const int &cidrprefix) {
    return pow(2, subnetprefix - cidrprefix);
}
// Find network class and  default mask  
void Address::findNetworkClass(const arrayInt &address, bool defaultmask, char netclass) {
    int netbit = 0;         // this is a dummy variable to store default mask for error message
    if ((address.at(0) >=   1 && address.at(0) <= 126) || (netclass == 'A')) {            // Class A Network
        num_of_network_bits = 8;
        network_class = 'A';
        netbit= 8;
        if (defaultmask == 1 || cidrmask_length == 0){              // find netclass mask too
            cidrmask= {{255,0,0,0}};
            cidrmask_length = 8;
        }
    }
    else if ((address.at(0) >= 128 && address.at(0) <= 191) || (netclass == 'B')) {       // Class B Network
        num_of_network_bits = 16;
        network_class = 'B';
        netbit= 16;
        if (defaultmask == 1 || cidrmask_length == 0){              // find netclass mask too
            cidrmask= {{255,255,0,0}};
            cidrmask_length = 16;
        }
    }
    else if ((address.at(0) >= 192 && address.at(0) <= 223) ||(netclass == 'C')) {       // Class C Network
        num_of_network_bits = 24;
        network_class = 'C';
        netbit= 24;
        if (defaultmask == 1 || cidrmask_length == 0){               // find netclass mask too
            cidrmask = {{255,255,255,0}};
            cidrmask_length = 24;
        }
    }
    else if (address.at(0) >= 224 && address.at(0) <= 239) {       // Class D Network
        network_class = 'D';                                        // Multicast address
        cerr << "ERROR!: Class 'D' is for multicast. Try different class.\n";
        exit(0);
    }
    else if (address.at(0) >= 240 && address.at(0) <= 254) {       // Class E Network
        network_class = 'E';                                        // Experimental address
        cerr << "ERROR!: Class 'E' if for experimental purposes. Try different class.\n";
        exit(0);
    }
    else {
        cerr << "ERROR!: Undefined Network\n";
        exit(1);
    }
    if (subnetmask_length < num_of_network_bits && netclass == '0') {
        cerr <<"ERROR!: [Classful Subnetting] Subnetmask of Class '" << network_class <<
            "' Network Must be >= " << netbit << '.' << endl;
        exit(1);
    }
}
// Find resident network/subnet of and address
arrayInt Address::findResidentSubnet(const arrayInt &address, const arrayInt &subnetmask, const arrayInt &cidrmask){
    arrayInt residentSubnet = {{0, 0, 0, 0}};
    int X = 0;                                                   //significant octet
    int interestingOctet = 0;
    int subIncNum = findSubIncNum(subnetmask, cidrmask);
    interestingOctet = findInterestingOctet(subnetmask);
    for (int i = 0; i <= 256; i +=subIncNum) {
        if (address.at(interestingOctet) < i) {
            X = i - subIncNum;
            break;
        }
    }
    if (interestingOctet == 0)
        residentSubnet = {{X, 0, 0, 0}};
    else if (interestingOctet == 1)
        residentSubnet = {{address[0], X, 0, 0}};
    else if (interestingOctet == 2)
        residentSubnet = {{address[0], address[1], X, 0}};
    else
        residentSubnet = {{address[0], address[1], address[2], X}};
    return residentSubnet;
}
// Calculate the position of the octet in which the increment will be taken place into.
int Address::findInterestingOctet(const arrayInt &subnetmask) {
    int interestingOctet = 0;
    int subnetMaskLength = findPrefix(subnetmask);
    if (subnetMaskLength >= 0 && subnetMaskLength <= 8)
        interestingOctet = 0;  // X.0.0.0
    else if (subnetMaskLength > 8 && subnetMaskLength <= 16)
        interestingOctet = 1;  // 0.X.0.0
    else if (subnetMaskLength > 16 && subnetMaskLength <= 24)
        interestingOctet = 2;  // 0.0.X.0
    else if (subnetMaskLength > 24 && subnetMaskLength <= 32)
        interestingOctet = 3;  // 0.0.0.X
    return interestingOctet;
}
// Convert 255.255.255.0 into /24
int Address::findPrefix(const arrayInt &mask) {
    bitset<8> oct1(mask[0]);
    bitset<8> oct2(mask[1]);
    bitset<8> oct3(mask[2]);
    bitset<8> oct4(mask[3]);
    return oct1.count() + oct2.count() + oct3.count() + oct4.count();
}
// Find subnet increment number 
int Address::findSubIncNum(const arrayInt &subnetmask, const arrayInt &cidrmask) {
    int subIncNum = 0;
    int interestingOctet = findInterestingOctet(subnetmask);
    if (subnetmask.at(interestingOctet) == cidrmask.at(interestingOctet))
        subIncNum = 256 - cidrmask.at(interestingOctet);
    else
        subIncNum = 256 - subnetmask.at(interestingOctet);
    return subIncNum;
}
// Convert ipv4/mask string into array 
arrayInt Address::fromString(const string &address) {
    arrayInt arr {{0,0,0,0}};
    if (address.size() < 7){
        cerr << "ERROR!: Invalid address/mask format '" << address << "'.\n";
        exit(1);
    }
    size_t pos = 0, delimiterPos, idx = 0;
    while((delimiterPos = address.find_first_of('.', pos)) != string::npos && idx < 4 && delimiterPos != pos) {
        arr[idx] = atoi(address.substr(pos, delimiterPos - pos).c_str());
        pos = delimiterPos+1;
        ++idx;
    }
    if (idx != 3 || address.substr(pos).empty()) {   // 192.168.1.1.1, 192...0, 192.168.1. 
        cerr << "ERROR!: Invalid address/mask format '" << address << "'.\n";
        exit(1);
    }
    arr[idx] = atoi(address.substr(pos).c_str());
    return arr;
}
