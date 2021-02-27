import React, { Component } from "react";
import { View } from "react-juce";
import SideBar from './SideBar';
import ParametricComponent from './ParametricComponent';

class App extends Component {
  render() {
    return (
      <View {...styles.root}>
        <View {...styles.body}>
          <View {...styles.mainDisplay}>
            <View {...styles.topBar} />
            <View {...styles.parametric}>
              <ParametricComponent />
            </View>
          </View>
          <View {...styles.bottomBar} />
        </View>
          <SideBar {...styles.sideBar} />
      </View>
    );
  }
}

const styles = {
  root: {
    padding: 4,
    backgroundColor: '#101213',
    width: '100%',
    height: '100%',
  },
  body: {
    width: '75%',
    height: '100%',
    flexDirection: 'column',
  },
  mainDisplay: {
    backgroundColor: '#202026',
    height: '80%',
    width: '100%',
    marginBottom: 4,
    flexDirection: 'column',
  },
  topBar: {
    height: '20%',
    width: '100%',
    backgroundColor: '#9916171b',
  },
  parametric: {
    height: '80%',
    width: '100%',
  },
  bottomBar: {
    height: '20%',
    width: '100%',
    backgroundColor: '#16171b',
  },
  sideBar: {
    backgroundColor: '#16171b',
    width: '25%',
    height: '100%',
    marginLeft: 4,
  },
};

export default App;
