import React from 'react';
import axios from 'axios';
import { Button } from 'react-bootstrap';

import { AuthContext, StoreContext } from '../../../contexts';

export function DeleteCode ({ presId, activeIndex, codeIndex, setSlides }) {
  const { storeData } = React.useContext(StoreContext);
  const { config } = React.useContext(AuthContext);

  const deleteCode = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const userPres = userStore.store.presentations;
      const presIndex = userPres.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });
      const currPres = userPres[presIndex];
      const currSlide = currPres.slides[activeIndex];

      currSlide.code.splice(codeIndex, 1);
      storeData(userStore);
      setSlides(currPres.slides);
    } catch (err) {
      alert(err.response);
    }
  }
  return (
    <Button variant="danger" onClick={deleteCode}>Delete Code</Button>
  )
}
