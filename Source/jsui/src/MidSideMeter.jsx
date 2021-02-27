import React from 'react';
import { View } from 'react-juce';

function _Meter(props) {
  return React.createElement("MeterComponent", props, props.children);
}

class MidSideMeter extends React.Component {
  render() {
    return (
      <View
        {...styles.root}
        {...this.props}
      >
        <_Meter
          colorUnfilled='#333'
          colorFilled='#fff'
          {...styles.meter}
        />
        <_Meter
          colorUnfilled='#333'
          colorFilled='#fff'
          {...styles.meter}
        />
      </View>
    )
  }
}

const styles = {
  root: {
    padding: 20,
  },
  meter: {
    width: '100%',
    height: '100%',
  },
};

export default MidSideMeter;
