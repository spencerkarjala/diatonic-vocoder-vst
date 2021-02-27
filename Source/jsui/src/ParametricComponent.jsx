import React from 'react';
import { View } from 'react-juce';

function _ParametricComponent(props) {
  return React.createElement("ParametricComponent", props, props.children);
}

class ParametricComponent extends React.Component {
  render() {
    return (
      <View
        {...styles.root}
        {...this.props}
      >
        <_ParametricComponent
          colors={{
            border: '#33af8fbf',
            grid: '#7333',
            parametricCurve: '#fff',
          }}
          {...styles.parametric}
        />
      </View>
    );
  }
}

const styles = {
  root: {
    width: '100%',
    height: '100%',
  },
  parametric: {
    width: '100%',
    height: '100%',
  },
};

export default ParametricComponent;
