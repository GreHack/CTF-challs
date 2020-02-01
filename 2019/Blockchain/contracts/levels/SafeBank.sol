pragma solidity ^0.5.8;

contract SafeBank{

    bool public cleared = false;


    struct Deposit{
        address signer;
        uint value;
    }

    mapping(bytes32 => Deposit) deposits;
    mapping(bytes32 => mapping(bytes32 => mapping(uint8 => bool))) signatures_used;
    
    uint public nonce;

    // Anyone can deposit money
    function deposit() payable public returns(bytes32){
        nonce += 1;
        bytes32 hash = get_hash(msg.sender, msg.value, nonce);

        // Store the deposit
        require(deposits[hash].signer == address(0));
        deposits[hash].signer = msg.sender;
        deposits[hash].value = msg.value;
        return hash;
    }

    // Helpers
    function get_hash(address from, uint value, uint deposit_nonce) pure public returns(bytes32){
        return keccak256(abi.encode(from, value, deposit_nonce));
    }

    /// Build a prefixed hash to copy the behavior of eth_sign.
    function prefixed(bytes32 hash) internal pure returns (bytes32) {
        return keccak256(abi.encodePacked("\x19Ethereum Signed Message:\n32", hash));
    }

    // To withdraw the funds, you need the depositor's approval
    function withdraw(bytes32 hash, bytes32 r, bytes32 s, uint8 v) public{
        require(deposits[hash].signer != address(0));

        // Recover the signer's address
        address signer = ecrecover(prefixed(hash), v, r, s);
        require(signer != address(0));
       
        // Prevent re-using the same signature multiple times 
        require(!signatures_used[r][s][v]);
        signatures_used[r][s][v] = true;

        // Require that the signer was the deposit's sender
        require(deposits[hash].signer == signer);
        msg.sender.transfer(deposits[hash].value);
        
    }

    function winChall() public{
        require(nonce != 0);
        require(address(this).balance == 0);

        cleared = true;
    }


}