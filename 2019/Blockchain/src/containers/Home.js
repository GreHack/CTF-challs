import React from 'react'
import {connect} from 'react-redux'
import {bindActionCreators} from 'redux'
import * as constants from '../constants'

class Home extends React.Component {

  navigateToFirstIncompleteLevel() {

    // Find first incomplete level
    let target = this.props.levels[0].deployedAddress
    for(let i = 0; i < this.props.levels.length; i++) {
      const level = this.props.levels[i]
      const completed = this.props.completedLevels[level.deployedAddress]
      if(!completed) {
        target = level.deployedAddress
        break
      }
    }

    // Navigate to first incomplete level
    this.props.router.push(`${constants.PATH_LEVEL_ROOT}${target}`)
  }

  render() {
    return (
      <div
        className="row"
        style={{
        paddingLeft: '40px',
        paddingRight: '40px',
      }}>

        <div className="col-sm-12">

          {/* TITLE */}
          <h2 className="title">
            The Ethernaut - GreHack Edition&nbsp;
          </h2>
          {/* INFO */}
          <p>The Ethernaut is a Web3/Solidity based wargame framework inspired on <a href="https://overthewire.org" target="_blank" rel="noopener noreferred">overthewire.org</a>, played in the Ethereum Virtual Machine. It was created by
        <a href='https://openzeppelin.com' target="_blank" rel="noopener noreferred">
        <img style={{ maxWidth: '120px' }} src='../../imgs/openzeppelin-by-logo.png' alt='OpenZeppelin'/>
        </a>
        </p>

        <p>
        Each challenge here will, once completed, give you a flag: write it
        somewhere safe, as rebooting our servers may cause the platform to lose track of which Ethereum challenge(s)
        you have already solved.
        To get points on the GreHack CTF platform, you need to come to the admin table with the flag and the name of the challenge.
        </p>

        <p>
        Instructions for how to start can be found on the first level.
        </p>
        <p>
        Good luck and have fun!
        </p>

        <button
            style={{marginTop: '10px'}}
            className="btn btn-primary"
            onClick={() => this.navigateToFirstIncompleteLevel()}
          >
            Play now! 
          </button>
        </div>
      </div>
    )
  }
}

function mapStateToProps(state) {
  return {
    levels: state.gamedata.levels,
    completedLevels: state.player.completedLevels
  };
}

function mapDispatchToProps(dispatch) {
  return bindActionCreators({

  }, dispatch);
}

export default connect(
  mapStateToProps,
  mapDispatchToProps
)(Home);
