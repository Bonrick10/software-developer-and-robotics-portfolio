import React from 'react';
import { Navigate } from 'react-router-dom';
import { Navbar, Row, Col } from 'react-bootstrap';

import { ListPresentation, LogoutButton, NewPresModal } from '../components/dashboard';
import { AuthContext, StoreProvider } from '../contexts';

export function Dashboard () {
  const { token, setLocalToken } = React.useContext(AuthContext);
  if (token === 'null') {
    // Ensures localStorage token is null when reaching login route.
    localStorage.setItem('token', 'null');
    return <Navigate to ="/login" />
  }
  const [modalShow, setModalShow] = React.useState(false);

  const handleClose = () => setModalShow(false);
  const handleOpen = () => setModalShow(true);

  return (
    <StoreProvider>
      <Navbar expand="lg" className="bg-body-tertiary">
        <Navbar.Brand>Presto</Navbar.Brand>
        <Row>
          <Col xs="auto">
            <NewPresModal show={modalShow} onHide={handleClose} onOpen={handleOpen}></NewPresModal>
          </Col>
          <Col xs="auto">
            <LogoutButton setLocalToken={setLocalToken}/>
          </Col>
        </Row>
      </Navbar>
      <h1>Dashboard</h1>
      <ListPresentation/><br />
    </StoreProvider>
  )
}
