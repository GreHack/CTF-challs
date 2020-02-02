const DepositFactory = artifacts.require('DepositFactory.sol')
const Deposit = artifacts.require('Deposit')
const DepositAttack = artifacts.require('DepositAttack')

const Ethernaut = artifacts.require('./Ethernaut.sol')

import * as utils from '../utils/TestUtils'
const truffleAssert = require('truffle-assertions');

const { expectRevert } = require('@openzeppelin/test-helpers');

contract('Deposit', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];
    //let initialDeposit = web3.utils.toWei("1", "ether")

    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await DepositFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Deposit, {value: web3.utils.toWei("1", "ether"), from:player})

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
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Deposit, {value: web3.utils.toWei("1", "ether"), from:player})
        
        const balance = await  web3.eth.getBalance(instance.address)
        assert.isTrue(balance == web3.utils.toWei("1", "ether"))

        const attacker = await DepositAttack.new()
        await attacker.launch_attack(instance.address, 
            {value: web3.utils.toWei("1", "ether"), from:player})
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
