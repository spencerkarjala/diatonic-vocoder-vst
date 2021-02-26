const path = require('path');

console.log(path.resolve('./src/index.js'));
console.log(path.resolve('./build/js'));

module.exports = {
  entry: path.resolve('./src/index.js'),
  output: {
    path: path.resolve("./build/js"),
    filename: "main.js",
    sourceMapFilename: "[file].map",
    devtoolModuleFilenameTemplate: (info) =>
      `webpack:///${info.absoluteResourcePath.replace(/\\/g, "/")}`,
  },
  devtool: "source-map",
  watchOptions: {
    poll: true,
    ignored: /node_modules/,
  },
  resolve: {
    extensions: ['.ts', '.tsx', '.js', '.jsx'],
  },
  module: {
    rules: [
      {
        test: /\.(js|jsx)$/,
        exclude: /node_modules/,
        use: ["babel-loader"],
      },
      // {
      //   test: /\.(ts|tsx)?$/,
      //   exclude: /node_modules/,
      //   use: 'ts-loader',
      // },
      {
        test: /\.svg$/,
        exclude: /node_modules/,
        use: ["svg-inline-loader"],
      },
    ],
  },
};
