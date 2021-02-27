import React from 'react';
import { View } from 'react-juce';

function _FloorCeilingSlider(props) {
  return React.createElement("FloorCeilingSliderComponent", props, props.children);
}

class FloorCeilingSlider extends React.Component {
  render() {
    return (
      <View
        {...styles.root}
        {...this.props}
      >
        <_FloorCeilingSlider
          width='100%'
          height='100%'
        />
      </View>
    );
  }
}

const styles = {
  root: {
    width: '100%',
    height: '100%',
    padding: 20,
  },
};

export default FloorCeilingSlider;
