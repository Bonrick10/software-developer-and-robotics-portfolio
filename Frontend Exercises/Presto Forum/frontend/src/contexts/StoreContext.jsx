import React from 'react';
import axios from 'axios';
import { AuthContext } from '.';
const StoreContext = React.createContext();

const StoreProvider = ({ children }) => {
  const { config } = React.useContext(AuthContext);

  const storeData = async (store) => {
    try {
      await axios.put('http://localhost:5005/store', store, config);
    } catch (err) {
      alert(err.response.data.error);
    }
  }

  return (
    <StoreContext.Provider value={{ storeData }}>
      {children}
    </StoreContext.Provider>
  )
}

export { StoreContext, StoreProvider };
