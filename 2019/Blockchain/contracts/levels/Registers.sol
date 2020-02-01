    pragma solidity ^0.5.8;

    contract Registers{

        bool public cleared = false;
        address public owner;
        mapping(address => bytes[]) public registers; 

        constructor () public{
            bytes[] storage register = registers[msg.sender];
            register.push("This is my immutable info!");
            owner = msg.sender;
        }

        // Allow anyone to register new information
        // We limit to 255 entry per address to prevent abuse
        function registerInfo(bytes memory info) public {
            bytes[] storage register = registers[msg.sender];
            // Spam mitigation
            require(register.length < 255);
            register.push(info);
        }

        // Update an existing info entry
        // Only the owner can do it
        function updateInfo(uint index, bytes memory new_info) public {
            bytes[] storage register = registers[msg.sender];

            // Solidity prevents out-of-bonds access, so no need to worry here
            register[index] = new_info;
        }

        // Allow to clean some of the info 
        // Only the owner can do it
        function removeInfos(uint number_of_elements_to_remove) public {
            bytes[] storage register = registers[msg.sender];
            require(register.length <= number_of_elements_to_remove);
            register.length -= number_of_elements_to_remove;
        }

        function winChall() public{
            require(registers[owner].length == 0);

            cleared = true;
        }

    }