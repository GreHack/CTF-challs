pragma solidity ^0.5.8;

contract FundMe{

    string _my_name;
    address payable public owner;

    constructor(string memory my_name) public{
        owner = msg.sender;
        _my_name = my_name;
    }

    function my_name() public returns(string memory){
        return _my_name;
    }

    // To allow receiving funds
    function () payable external{}

    function withdraw() public{
        owner.transfer(address(this).balance);
    }

}

contract CrowdFunding{

    bool public cleared = false;

    mapping(string => uint) public funds;
    mapping(string => FundMe) public candidates;

    function register(FundMe candidate) public {
        // Prevent dupplicate registrations
        require(address(candidates[candidate.my_name()]) == address(0x0));
        candidates[candidate.my_name()] = candidate;
    }

    function fund(string memory name) public payable{
        require(address(candidates[name]) != address(0x0));
        funds[name] += msg.value;
    }

    function withdraw_funds(string memory name) public{
        FundMe candidate = candidates[name];

        // require the caller to be the contract's owner
        require(msg.sender == candidate.owner());

        uint balance = funds[name];

        // Ensure the candidate did not changed his name
        // Because smart contracts are the future of the world
        // The cheapest way to do string comparison
        // Is to compare their hashes
        require(keccak256(abi.encode(candidate.my_name())) == keccak256(abi.encode(name)));
        
        funds[name] = 0;
        // Use transfer to prevent reentrancy
        address(candidate).transfer(balance);
    }

    function winChall() public{
        require(address(this).balance == 0);

        cleared = true;
    }


}