import React from 'react';
import axios from 'axios';
import { useNavigate } from 'react-router-dom';
import { Navbar, Container, Button } from 'react-bootstrap';

import { EditTitleModal } from '.';
import { AuthContext, StoreContext } from '../../contexts';

export function EditNavBar ({ presId }) {
  const { config } = React.useContext(AuthContext);
  const { storeData } = React.useContext(StoreContext);

  const [pres, setPres] = React.useState({});

  React.useEffect(() => {
    const getData = async () => {
      try {
        const response = await axios.get('http://localhost:5005/store', config);
        const userStore = response.data.store;
        const thisPres = userStore.store.presentations.find((currPres) => {
          // temporary check given that backend contains presentation objects without presIds
          if (currPres.presId === undefined) {
            return false;
          }
          return (currPres.presId.toString() === presId)
        });
        if (!thisPres) {
          alert('Presentation ID not found');
        }
        setPres(thisPres);
      } catch (err) {
        console.log(err);
        alert(err.response);
      }
    }
    getData();
  }, [storeData]);

  const deletePres = async () => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) =>
        (currPres.presId.toString() === presId)
      );

      userStore.store.presentations.splice(presIndex, 1);
      storeData(userStore);

      returnToDash();
    } catch (err) {
      console.log(err);
      alert(err.response);
    }
  }

  const navigate = useNavigate();
  const returnToDash = () => {
    navigate('/dashboard');
  }

  const editTitle = async (newTitle) => {
    try {
      const response = await axios.get('http://localhost:5005/store', config);
      const userStore = response.data.store;
      const presIndex = userStore.store.presentations.findIndex((currPres) => {
        return (currPres.presId.toString() === presId)
      });

      userStore.store.presentations[presIndex].title = newTitle;
      storeData(userStore);
      navigate(`/editPres/${presId}`);
    } catch (err) {
      console.log(err);
      alert(err.response);
    }
  }

  const [modalShow, setModalShow] = React.useState(false);

  const handleClose = () => setModalShow(false);
  const handleOpen = () => setModalShow(true);

  return (
    <>
    <Navbar className="bg-body-tertiary justify-content-between">
      <Container>
        <Navbar.Brand>
            {pres.title}
        </Navbar.Brand>
        <EditTitleModal show={modalShow} onHide={handleClose} onOpen={handleOpen} currTitle={pres.title} editTitle={editTitle}></EditTitleModal>
      </Container>
      <Container>
        <Button variant="danger" onClick={deletePres}>
          Delete
        </Button>
        <Button variant="secondary" onClick={returnToDash}>
          Back
        </Button>
      </Container>
    </Navbar>
    </>
  )
}
