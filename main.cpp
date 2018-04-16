#include <iostream>
#include <getopt.h>
#include "ipv4subnetting.hpp"

using std::cout;
using std::cerr;
using std::endl;

// Main Function
int main (int argc, char *argv[]) {
    if ( argc == 1 ) {
        cerr << "subcal: Missing options\n";
        cout << "Try 'subcal help' for more information\n";
        exit(1);
    }
    // Manditory positional options.
    std::array<string, 4> mandOpts = {{"info", "gen", "design", "help"}};
    string first_arg;
    if (argc > 8) {
        cerr << "ERROR!: Too many arguments\n";
        exit(1);
    }
    // Check if any option was provided
    for (int i = 0; i < argc && argc > 2; i++) {
        if (string(argv[i]).substr(0,1) == "-"){
            break;
        }
        else if (i == argc-1) {
            cerr << "ERROR!: No options were provided.\n";
            exit(1);
        }

    }
    // First argument must be a manditory option listed in mandOpts vector.
    if (argc > 1) {
        first_arg = string(argv[1]);
        for (size_t i=0; i < mandOpts.size(); i++) {
            if (mandOpts.at(i) == first_arg)
                break;
            else if (i == mandOpts.size()-1) {
                cerr << "ERROR!: Unrecognized option '" << first_arg << "'." <<endl;
                exit(1);
            }
        }
    }
    // Check if the manditory option followed by an option.
    if (argc > 2 && string(argv[2]).substr(0,1) != "-") {
        cerr << "ERROR!: Unrecognized option '" << argv[2] << "'\n";
        exit(1);
    }
    // Init subcal options
    struct Flags {
        bool ipv4addr        = false;
        bool ipv6addr        = false;
        bool subnetmask      = false;
        bool prefix          = false;
        bool numhosts        = false;
        bool numsubnets      = false;
        bool netclass        = false;
        bool verbose         = false;
    };
    // For long options that have no equivalent short option.
    enum {
        ipv4=1000,
        ipv6
        };
    const char* const short_opts = "o:m:p:s:h:c:v";
    static struct option const long_opts[] = {
        {"ip4", required_argument, nullptr, ipv4},
        {"ip6", required_argument, nullptr, ipv6},
        {"subnets", required_argument, nullptr, 's'},
        {"hosts", required_argument, nullptr, 'h'},
        {"class", required_argument, nullptr, 'c'},
        {"verbose", no_argument, nullptr, 'v'},
        {nullptr, 0, nullptr, 0}
    };
    Address address;
    struct Flags flag;
    int opt = 0;
    while ((opt = getopt_long(argc, argv, short_opts, long_opts, nullptr)) != -1) {
        switch (opt) {
            case ipv4 :
                if (argc-optind > 0 && string(argv[optind]).substr(0,1) != "-") {
                    address.isValidAddr(optarg, "cidrmask");
                    address.isValidMask(argv[optind], "subnetmask");
                }
                else {
                    address.isValidAddr(optarg, "subnetmask");
                }
                flag.ipv4addr = true;
                break;
            case 'v' :
                flag.verbose = true;
                break;
            case 's' :
                flag.numsubnets = true;
                address.isValidDesign(optarg, "subnets");
                break;
            case 'h' :
                flag.numhosts = true;
                address.isValidDesign(optarg, "hosts");
                break;
            case 'c' :
                flag.netclass = true;
                address.isValidDesign(optarg, "netclass");
                break;
            case '?' :
                exit(1);
                break;
            default:
                cerr << "Try 'subcal help' for more information."<< endl;
                exit(1);
        }
    }
    if (argc > 1 && first_arg == mandOpts.at(0)) {
        if (flag.ipv4addr) {
            address.findNetworkClass(address.getIPv4Address());
            address.subnetDetails(address.getIPv4Address(), address.getSubnetMask(), address.getCidrMask());
            exit(0);
        }
        else {
            cerr << "ERROR!: '" << mandOpts.at(0) << "' required '--ip4' option.\n";
        }
    }
    else if (argc > 1 && first_arg == mandOpts.at(1)) {
        if (flag.ipv4addr) {
            address.findNetworkClass(address.getIPv4Address());
            if (flag.verbose)
                address.subnetDetails(address.getIPv4Address(), address.getSubnetMask(), address.getCidrMask());
            address.generateSubnets(address.getIPv4Address(), address.getSubnetMask(), address.getCidrMask(), flag.verbose);
            exit(0);
        }
        else {
            cerr << "ERROR!: '" << mandOpts.at(1) << "' required '--ip4' option.\n";
            exit(1);
        }
    }
    else if ( argc > 1 && first_arg == mandOpts.at(2)) {
        if (flag.numsubnets && flag.numhosts && flag.netclass) {
            address.findAllMasks(address.getNetClass(), address.getNumOfSubs(), address.getNumOfHosts());
            exit(0);
        }
        else {
            cerr << "ERROR!: 'desing' required '-s,-h,-c' options.\n";
            exit(1);
        }
    }
    else if (argc > 1 && string(argv[1]) == mandOpts.at(3)){
        address.printUsage();
        exit(0);
    }
    else if (argc > 1){
        cerr << "ERROR!: No arguments were detected '"<< first_arg <<"'.\n";
        exit(1);
    }
    cout << endl;
    return 0;
}
